#pragma once

#include "Entity.hpp"
#include "GameStats.hpp"
#include <atomic>
#include <memory>
#include <thread>

namespace rngine {
class GameLoop {
public:
  GameLoop(std::vector<Entity> _entities);
  ~GameLoop();
  void pause();
  void resume();
  std::vector<Entity> getEntitiesSnapshot();

private:
  std::vector<Entity> _entities;
  std::atomic<bool> _isRunning{true};
  std::atomic<bool> _isPaused{true};
  GameStats _gameStats;
  std::unique_ptr<std::thread> _gameThread;

  static constexpr double TARGET_DELTA_TIME = 1.0 / 60.0;

  void runGameLoop();
  void update(double deltaTime);
  void updateStats(double deltaTime);
};
} // namespace rngine
