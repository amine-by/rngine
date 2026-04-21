#include "GameLoop.hpp"
#include "Entity.hpp"
#include <android/log.h>
#include <chrono>
#include <cinttypes>

namespace rngine {

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

void GameLoop::initialize(std::vector<Entity> entities) {
  std::lock_guard<std::mutex> lock(_mutex);
  _entities = std::move(entities);
  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Initialized %zu entities",
                      _entities.size());
}

void GameLoop::pause() {
  if (_isPaused) {
    __android_log_print(ANDROID_LOG_INFO, "GameLoop",
                        "Already paused, ignoring");
    return;
  }

  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Pause");
  _isPaused.store(true);
}

void GameLoop::resume() {
  if (!_isPaused) {
    __android_log_print(ANDROID_LOG_INFO, "GameLoop",
                        "Already running, ignoring");
    return;
  }

  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Resume");
  _isPaused.store(false);
}

std::vector<Entity> GameLoop::getEntitiesSnapshot() {
  std::lock_guard<std::mutex> lock(_mutex);
  return _entities;
}

void GameLoop::runGameLoop() {
  using namespace std::chrono;

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

      while (accumulator >= TARGET_DELTA_TIME) {
        update(TARGET_DELTA_TIME);
        accumulator -= TARGET_DELTA_TIME;
      }
    }

    std::this_thread::sleep_for(milliseconds(1));
  }

  __android_log_print(ANDROID_LOG_INFO, "GameLoop", "Game loop thread stopped");
}

void GameLoop::update(double deltaTime) {
  updateStats(deltaTime);
  updateEntities(deltaTime);
}

Entity *GameLoop::getEntityById(const std::string &id) {
  std::lock_guard<std::mutex> lock(_mutex);

  for (auto &entity : _entities) {
    if (std::strncmp(entity.id, id.c_str(), ENTITY_ID_SIZE) == 0) {
      return &entity;
    }
  }
  return nullptr;
}

void GameLoop::updateStats(double deltaTime) {
  static double timeAccumulator = 0.0;
  static uint64_t frameCount = 0;

  timeAccumulator += deltaTime;
  frameCount++;

  if (timeAccumulator >= 1.0) {
    _gameStats.fps = frameCount;
    _gameStats.deltaTime = deltaTime;

    __android_log_print(ANDROID_LOG_DEBUG, "GameLoop",
                        "FPS: %.2f, Delta: %.4f, Total Frames: %" PRIu64,
                        _gameStats.fps, _gameStats.deltaTime,
                        _gameStats.totalFrames);

    timeAccumulator = 0.0;
    frameCount = 0;
  }

  _gameStats.totalFrames++;
}

void GameLoop::updateEntities(double deltaTime) {
  std::lock_guard<std::mutex> lock(_mutex);

  for (auto &entity : _entities) {
    entity.px += entity.vx * deltaTime;
    entity.py += entity.vy * deltaTime;
  }
}
} // namespace rngine
