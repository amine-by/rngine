#pragma once

#include "Entity.hpp"
#include "GameStats.hpp"
#include "Rect.hpp"
#include <atomic>
#include <memory>
#include <thread>

namespace margelo::nitro::rngine {
class GameLoop {
public:
  static GameLoop &getInstance();

  GameLoop(const GameLoop &) = delete;
  GameLoop &operator=(const GameLoop &) = delete;
  GameLoop(GameLoop &&) = delete;
  GameLoop &operator=(GameLoop &&) = delete;

  ~GameLoop();

  std::vector<Entity> &getEntitiesInternal() { return _entities; };
  std::atomic<bool> &isPausedInternal() { return _isPaused; };
  std::mutex &getMutexInternal() { return _mutex; };

  Entity *findEntityInternal(const std::string &id);
  std::vector<Rect> getRectsSnapshot();

private:
  explicit GameLoop();
  std::mutex _mutex;
  std::vector<Entity> _entities{};
  std::atomic<bool> _isRunning{true};
  std::atomic<bool> _isPaused{true};
  GameStats _gameStats;
  std::unique_ptr<std::thread> _gameThread;

  static constexpr double TARGET_DELTA_TIME = 1.0 / 60.0;

  void runGameLoop();
  void update(double deltaTime);
  void updateStats(double deltaTime);
  void updateEntities(double deltaTime);
};
} // namespace margelo::nitro::rngine
