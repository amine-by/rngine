#include "GameLoop.hpp"
#include <android/log.h>
#include <chrono>
#include <cinttypes>

namespace margelo::nitro::rngine {

GameLoop::GameLoop() : HybridObject(TAG) {
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
  // TODO: Add game logic here!
}

void GameLoop::updateStats(double deltaTime) {
  static double timeAccumulator = 0.0;
  static uint64_t frameCount = 0;

  timeAccumulator += deltaTime;
  frameCount++;

  if (timeAccumulator >= 1.0) {
    _stats.fps = frameCount;
    _stats.deltaTime = deltaTime;

    timeAccumulator = 0.0;
    frameCount = 0;
  }

  _stats.totalFrames++;

  __android_log_print(ANDROID_LOG_DEBUG, "GameLoop",
                      "FPS: %.2f, Delta: %.4f, Total Frames: %" PRIu64,
                      _stats.fps, _stats.deltaTime, _stats.totalFrames);
}

} // namespace margelo::nitro::rngine
