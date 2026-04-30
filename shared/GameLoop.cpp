#include "GameLoop.hpp"
#include "ColorUtils.hpp"
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

std::vector<Rect> GameLoop::getRectsSnapshot() {
  std::lock_guard<std::mutex> lock(_mutex);
  std::vector<Rect> rects;
  rects.reserve(_entities.size() + 1);

  rects.push_back(
      {0, _world.width, 0, _world.height, parseHexColor(_world.color)});

  for (const auto &[id, entity] : _entities) {
    if (entity.px + entity.width < 0 || entity.px > _world.width ||
        entity.py + entity.height < 0 || entity.py > _world.height) {
      continue;
    }

    rects.push_back(
        {std::clamp(entity.px, 0.0, _world.width),
         std::clamp(entity.px + entity.width, 0.0, _world.width),
         std::clamp(entity.py, 0.0, _world.height),
         std::clamp(entity.py + entity.height, 0.0, _world.height),
         parseHexColor(entity.color)});
  }
  return rects;
}

void GameLoop::runGameLoop() {
  using namespace std::chrono;
  double targetDeltaTime = 1.0 / _tickRate;

  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Game loop thread started");

  auto previousTime = steady_clock::now();
  double accumulator = 0.0;

  while (_isRunning) {
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
  for (auto &system : _systems) {
    std::vector<Entity> entities;

    for (const auto &id : system.ids) {
      auto resolvedEntitiesInternal = resolveEntitiesInternal(id);
      for (auto *entity : resolvedEntitiesInternal)
        entities.push_back(*entity);
    }

    system.onTick(entities);
  }
}

void GameLoop::update(double deltaTime) {
  updateStats(deltaTime);
  updateEntities(deltaTime);
  runSystems();
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
    entity.px += entity.vx * deltaTime;
    entity.py += entity.vy * deltaTime;
  }
}
} // namespace margelo::nitro::rngine
