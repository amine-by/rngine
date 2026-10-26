#include "GameLoop.hpp"
#include "AssetUtils.hpp"
#include "Collision.hpp"
#include "CollisionUtils.hpp"
#include "Entity.hpp"
#include "GameRenderer.hpp"
#include "SystemContext.hpp"
#include <android/log.h>
#include <chrono>
#include <cinttypes>
#include <mutex>

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

void GameLoop::runGameLoop() {
  using namespace std::chrono;

  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Game loop thread started");

  auto previousTime = steady_clock::now();
  double accumulator = 0.0;

  auto &gameRenderer = GameRenderer::getInstance();

  while (_isRunning) {
    gameRenderer.processPendingTasks();

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

    {
      std::lock_guard<std::mutex> lock(_mutex);
      gameRenderer.render(_screen, _entities);
    }

    std::this_thread::sleep_for(milliseconds(1));
  }

  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Game loop thread stopped");
}

void GameLoop::runSystems() {
  for (size_t systemIndex = 0; systemIndex < _systems.size(); ++systemIndex) {
    auto &system = _systems[systemIndex];
    SystemContext systemContext;

    if (system.world.value_or(false)) {
      systemContext.world = _world;
    }

    if (system.screen.value_or(false)) {
      systemContext.screen = _screen;
    }

    if (system.entities.has_value()) {
      systemContext.entities.emplace();
      for (const auto &id : system.entities.value()) {
        auto resolvedEntitiesInternal = resolveEntitiesInternal(id);
        for (auto *entity : resolvedEntitiesInternal)
          systemContext.entities->push_back(*entity);
      }
    }

    if (system.collisions.has_value()) {
      systemContext.collisions.emplace();
      for (const auto &pair : system.collisions.value()) {
        for (const auto &c : _collisions) {

          if (entityIdMatches(c.a, pair.a) && entityIdMatches(c.b, pair.b)) {
            systemContext.collisions.value().push_back(c);
          } else if (entityIdMatches(c.a, pair.b) &&
                     entityIdMatches(c.b, pair.a)) {
            auto swappedCollision = c;

            swappedCollision.a = c.b;
            swappedCollision.b = c.a;

            swappedCollision.nx = -c.nx;
            swappedCollision.ny = -c.ny;

            systemContext.collisions->push_back(swappedCollision);
          }
        }
      }
    }

    std::shared_ptr<Promise<void>> promise = system.onTick(systemContext);
    promise->await().get();
  }
}

void GameLoop::update(double deltaTime) {
  updateStats(deltaTime);
  updateEntities(deltaTime);
  computeCollisions();
  resolveCollisions();
  runSystems();
}

void GameLoop::updateStats(double deltaTime) {
  static double timeAccumulator = 0.0;
  static uint64_t frameCount = 0;

  timeAccumulator += deltaTime;
  frameCount++;

  if (timeAccumulator >= 1.0) {
    _gameStats.tickRate = static_cast<double>(frameCount) / timeAccumulator;
    _gameStats.deltaTime = timeAccumulator / static_cast<double>(frameCount);

    __android_log_print(
        ANDROID_LOG_DEBUG, "GameLoop",
        "updateStats: TickRate: %.2f, Delta: %.4f, Total Ticks: %" PRIu64,
        _gameStats.tickRate, _gameStats.deltaTime, _gameStats.totalTicks);

    timeAccumulator -= 1.0;
    frameCount = 0;
  }

  _gameStats.totalTicks++;
}

void GameLoop::updateScreen(double deltaTime) {
  std::lock_guard<std::mutex> lock(_mutex);
  if (_screen.asset != 0) {
    AssetUtils::updateProgress(_screen.progress, _screen.speed, _screen.loop,
                               _screen.asset, deltaTime);
  }
}

void GameLoop::updateEntities(double deltaTime) {
  std::lock_guard<std::mutex> worldLock(_worldMutex);
  std::lock_guard<std::mutex> lock(_mutex);

  for (auto &[id, entity] : _entities) {

    if (entity.mass > 0.0) {
      entity.vx = entity.vx + _world.gx * deltaTime;
      entity.vy = entity.vy + _world.gy * deltaTime;
    }

    entity.vx = entity.vx + entity.ax * deltaTime;
    entity.vy = entity.vy + entity.ay * deltaTime;

    entity.px += entity.vx * deltaTime;
    entity.py += entity.vy * deltaTime;

    AssetUtils::updateProgress(entity.progress, entity.speed, entity.loop,
                               entity.asset, deltaTime);
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

    if (a.isSensor || b.isSensor)
      continue;

    double inverseMassA = a.mass > 0.0 ? 1.0 / a.mass : 0.0;
    double inverseMassB = b.mass > 0.0 ? 1.0 / b.mass : 0.0;
    double totalInverseMass = inverseMassA + inverseMassB;
    if (totalInverseMass == 0.0)
      continue;

    double correctionDepth = std::max(c.depth - slop, 0.0);
    double correction = (correctionDepth / totalInverseMass) * percent;
    a.px -= c.nx * correction * inverseMassA;
    a.py -= c.ny * correction * inverseMassA;
    b.px += c.nx * correction * inverseMassB;
    b.py += c.ny * correction * inverseMassB;

    double avx = a.vx, avy = a.vy;
    double bvx = b.vx, bvy = b.vy;
    double relativeVx = bvx - avx, relativeVy = bvy - avy;
    double velocityAlongNormal = relativeVx * c.nx + relativeVy * c.ny;
    if (velocityAlongNormal > 0)
      continue;

    double restitution = restitution = std::max(a.restitution, b.restitution);

    double impulse =
        -(1.0 + restitution) * velocityAlongNormal / totalInverseMass;

    a.vx = avx - impulse * c.nx * inverseMassA;
    a.vy = avy - impulse * c.ny * inverseMassA;
    b.vx = bvx + impulse * c.nx * inverseMassB;
    b.vy = bvy + impulse * c.ny * inverseMassB;
  }
}
} // namespace margelo::nitro::rngine
