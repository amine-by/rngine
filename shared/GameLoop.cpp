#include "GameLoop.hpp"
#include "Collision.hpp"
#include "CollisionUtils.hpp"
#include "Entity.hpp"
#include <android/log.h>
#include <chrono>
#include <cinttypes>
#include <set>

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
    double targetDeltaTime = 1.0 / _tickRate.load();
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
      std::set<std::pair<std::string, std::string>> uniqueEntityIdPairs;

      for (const auto &[a, b] : system.collisions.value()) {
        auto resolvedEntitiesInternalA = resolveEntitiesInternal(a);
        auto resolvedEntitiesInternalB = resolveEntitiesInternal(b);

        for (const auto &entityA : resolvedEntitiesInternalA) {
          for (const auto &entityB : resolvedEntitiesInternalB) {
            if (entityA == entityB)
              continue;

            auto uniqueEntityIdPairKey = std::minmax(entityA->id, entityB->id);

            if (uniqueEntityIdPairs.count(uniqueEntityIdPairKey))
              continue;

            uniqueEntityIdPairs.insert(uniqueEntityIdPairKey);

            if (auto ov = CollisionUtils::shapeOverlap(*entityA, *entityB)) {
              collisions.push_back(
                  Collision(entityA->id, entityB->id, ov.value()));
            }
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
  std::lock_guard<std::mutex> lock(_mutex);

  for (auto &[id, entity] : _entities) {
    if (entity.vx) {
      entity.px += *entity.vx * deltaTime;
    }

    if (entity.vy) {
      entity.py += *entity.vy * deltaTime;
    }

    if (entity.asset && entity.asset < 0) {
      auto it = _lottieDurations.find(*entity.asset);
      if (it != _lottieDurations.end() && it->second > 0.0) {
        entity.progress =
            fmod(entity.progress.value_or(0.0) + deltaTime / it->second, 1.0);
      }
    }
  }
}
} // namespace margelo::nitro::rngine
