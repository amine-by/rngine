#include "GameLoop.hpp"
#include "Collision.hpp"
#include "CollisionUtils.hpp"
#include "Entity.hpp"
#include <android/log.h>
#include <chrono>
#include <cinttypes>

namespace margelo::nitro::rngine {

GameLoop &GameLoop::getInstance() {
  static GameLoop instance;
  return instance;
}

GameLoop::GameLoop() {
  __android_log_print(ANDROID_LOG_INFO, "GameLoop",
                      "Constructor - Starting game thread");
  _gameThread = std::make_unique<std::thread>(&GameLoop::runGameLoop, this);
}

GameLoop::~GameLoop() {
  __android_log_print(ANDROID_LOG_INFO, "GameLoop",
                      "Destructor - Stopping game thread");

  _isRunning.store(false);

  if (_gameThread && _gameThread->joinable()) {
    _gameThread->join();
  }
  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Destructor - Complete");
}

bool GameLoop::entityIdMatches(const std::string &entityId,
                               const std::string &prefix) {
  return entityId == prefix || entityId.starts_with(prefix + "_");
}

std::vector<Entity *>
GameLoop::resolveEntitiesInternal(const std::string &prefix) {
  std::vector<Entity *> results;

  auto exact = _entities.find(prefix);
  if (exact != _entities.end()) {
    results.push_back(&exact->second);
    return results;
  }

  std::string pattern = prefix + "_";
  for (auto it = _entities.lower_bound(pattern); it != _entities.end(); ++it) {
    if (it->first.rfind(pattern, 0) != 0)
      break;
    results.push_back(&it->second);
  }

  return results;
}

void GameLoop::registerLottieDuration(double id, double duration) {
  std::lock_guard<std::mutex> lock(_mutex);
  _lottieDurations[id] = duration;
  __android_log_print(ANDROID_LOG_DEBUG, "GameLoop",
                      "Registered lottie duration: id: %.0f, duration: %.4f",
                      id, duration);
}

void GameLoop::runGameLoop() {
  using namespace std::chrono;

  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Game loop thread started");

  auto previousTime = steady_clock::now();
  double accumulator = 0.0;

  while (_isRunning) {
    double targetDeltaTime;
    {
      std::lock_guard<std::mutex> lock(_worldMutex);
      targetDeltaTime = 1.0 / _world.tickRate;
    }
    auto currentTime = steady_clock::now();
    double frameTime = duration<double>(currentTime - previousTime).count();
    previousTime = currentTime;

    if (frameTime > 0.25) {
      frameTime = 0.25;
    }

    if (!_isPaused) {
      accumulator += frameTime;

      while (accumulator >= targetDeltaTime) {
        update(targetDeltaTime);
        accumulator -= targetDeltaTime;
      }
    }

    std::this_thread::sleep_for(milliseconds(1));
  }

  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Game loop thread stopped");
}

void GameLoop::runSystems() {
  for (size_t systemIndex = 0; systemIndex < _systems.size(); ++systemIndex) {
    auto &system = _systems[systemIndex];
    std::vector<Entity> entities;
    std::vector<Collision> collisions;

    if (system.entities.has_value()) {
      for (const auto &id : system.entities.value()) {
        auto resolvedEntitiesInternal = resolveEntitiesInternal(id);
        for (auto *entity : resolvedEntitiesInternal)
          entities.push_back(*entity);
      }
    }

    if (system.collisions.has_value()) {
      for (const auto &pair : system.collisions.value()) {
        for (const auto &c : _collisions) {
          bool matchesForward =
              entityIdMatches(c.a, pair.a) && entityIdMatches(c.b, pair.b);
          bool matchesReverse =
              entityIdMatches(c.a, pair.b) && entityIdMatches(c.b, pair.a);
          if (matchesForward || matchesReverse) {
            collisions.push_back(c);
          }
        }
      }
    }

    __android_log_print(
        ANDROID_LOG_DEBUG, "GameLoop",
        "System Index: %zu, Entities: %zu, Collisions: %zu, Calling onTick",
        systemIndex, entities.size(), collisions.size());

    std::shared_ptr<Promise<double>> promise =
        system.onTick(entities, collisions);
    std::future<double> future = promise->await();
    double duration = future.get();

    __android_log_print(ANDROID_LOG_DEBUG, "GameLoop",
                        "System Index: %zu, onTick Complete, Duration: %.2fms",
                        systemIndex, duration);
  }
}

void GameLoop::captureSnapshot() {
  std::lock_guard<std::mutex> snapshotLock(_snapshotMutex);
  std::lock_guard<std::mutex> lock(_mutex);
  _entitiesSnapshot = _entities;
  _screenSnapshot = _screen;
}

void GameLoop::update(double deltaTime) {
  updateStats(deltaTime);
  updateEntities(deltaTime);
  computeCollisions();
  resolveCollisions();
  runSystems();
  captureSnapshot();
}

void GameLoop::updateStats(double deltaTime) {
  static double timeAccumulator = 0.0;
  static uint64_t frameCount = 0;

  timeAccumulator += deltaTime;
  frameCount++;

  if (timeAccumulator >= 1.0) {
    _gameStats.tickRate = static_cast<double>(frameCount);
    _gameStats.deltaTime = deltaTime;

    __android_log_print(ANDROID_LOG_DEBUG, "GameLoop",
                        "TickRate: %.2f, Delta: %.4f, Total Ticks: %" PRIu64,
                        _gameStats.tickRate, _gameStats.deltaTime,
                        _gameStats.totalTicks);

    timeAccumulator = 0.0;
    frameCount = 0;
  }

  _gameStats.totalTicks++;
}

void GameLoop::updateEntities(double deltaTime) {
  std::lock_guard<std::mutex> worldLock(_worldMutex);
  std::lock_guard<std::mutex> lock(_mutex);

  for (auto &[id, entity] : _entities) {

    if (entity.mass.has_value() && entity.mass.value() > 0.0) {
      if (_world.gx.has_value()) {
        entity.vx = entity.vx.value_or(0.0) + _world.gx.value() * deltaTime;
      }
      if (_world.gy.has_value()) {
        entity.vy = entity.vy.value_or(0.0) + _world.gy.value() * deltaTime;
      }
    }

    if (entity.ax.has_value()) {
      entity.vx = entity.vx.value_or(0.0) + entity.ax.value() * deltaTime;
    }
    if (entity.ay.has_value()) {
      entity.vy = entity.vy.value_or(0.0) + entity.ay.value() * deltaTime;
    }

    if (entity.vx.has_value()) {
      entity.px += entity.vx.value() * deltaTime;
    }
    if (entity.vy.has_value()) {
      entity.py += entity.vy.value() * deltaTime;
    }

    if (entity.asset.has_value() && entity.asset.value() < 0) {
      auto it = _lottieDurations.find(entity.asset.value());
      if (it != _lottieDurations.end() && it->second > 0.0) {
        entity.progress =
            fmod(entity.progress.value_or(0.0) + deltaTime / it->second, 1.0);
      }
    }
  }
}

void GameLoop::computeCollisions() {
  std::lock_guard<std::mutex> lock(_mutex);
  _collisions.clear();

  std::vector<Entity *> entities;
  entities.reserve(_entities.size());
  for (auto &[id, entity] : _entities) {
    entities.push_back(&entity);
  }

  for (size_t i = 0; i < entities.size(); ++i) {
    for (size_t j = i + 1; j < entities.size(); ++j) {
      auto result = CollisionUtils::shapeOverlap(*entities[i], *entities[j]);
      if (result.has_value()) {
        _collisions.push_back(result.value());
      }
    }
  }
}

void GameLoop::resolveCollisions() {
  std::lock_guard<std::mutex> lock(_mutex);

  const double percent = 0.8;
  const double slop = 0.01;

  for (const auto &c : _collisions) {
    auto itA = _entities.find(c.a);
    auto itB = _entities.find(c.b);
    if (itA == _entities.end() || itB == _entities.end())
      continue;

    Entity &a = itA->second;
    Entity &b = itB->second;

    if (a.isSensor.value_or(false) || b.isSensor.value_or(false))
      continue;

    double inverseMassA = (a.mass.has_value() && a.mass.value() > 0.0)
                              ? 1.0 / a.mass.value()
                              : 0.0;
    double inverseMassB = (b.mass.has_value() && b.mass.value() > 0.0)
                              ? 1.0 / b.mass.value()
                              : 0.0;
    double totalInverseMass = inverseMassA + inverseMassB;
    if (totalInverseMass == 0.0)
      continue;

    double correctionDepth = std::max(c.depth - slop, 0.0);
    double correction = (correctionDepth / totalInverseMass) * percent;
    a.px -= c.nx * correction * inverseMassA;
    a.py -= c.ny * correction * inverseMassA;
    b.px += c.nx * correction * inverseMassB;
    b.py += c.ny * correction * inverseMassB;

    double avx = a.vx.value_or(0.0), avy = a.vy.value_or(0.0);
    double bvx = b.vx.value_or(0.0), bvy = b.vy.value_or(0.0);
    double relativeVx = bvx - avx, relativeVy = bvy - avy;
    double velocityAlongNormal = relativeVx * c.nx + relativeVy * c.ny;
    if (velocityAlongNormal > 0)
      continue;

    double restitution = 0.0;
    double impulse =
        -(1 + restitution) * velocityAlongNormal / totalInverseMass;

    a.vx = avx - impulse * c.nx * inverseMassA;
    a.vy = avy - impulse * c.ny * inverseMassA;
    b.vx = bvx + impulse * c.nx * inverseMassB;
    b.vy = bvy + impulse * c.ny * inverseMassB;
  }
}
} // namespace margelo::nitro::rngine
