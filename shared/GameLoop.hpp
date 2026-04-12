#pragma once

#include <atomic>
#include <memory>
#include <thread>

namespace rngine {

class GameLoop {
public:
  GameLoop();
  ~GameLoop();
  void pause();
  void resume();

private:
  std::atomic<bool> _isRunning{true};
  std::atomic<bool> _isPaused{false};

  struct GameStats {
    double fps = 0.0;
    double deltaTime = 0.0;
    uint64_t totalFrames = 0;
  };

  GameStats _stats;

  std::unique_ptr<std::thread> _gameThread;

  static constexpr double TARGET_DELTA_TIME = 1.0 / 60.0;

  void runGameLoop();

  void update(double deltaTime);

  void updateStats(double deltaTime);
};
} // namespace rngine
