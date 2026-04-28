#pragma once

#include "Entity.hpp"
#include "GameStats.hpp"
#include "Rect.hpp"
#include "System.hpp"
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

  std::mutex &getMutexInternal() { return _mutex; };
  std::map<string, Entity> &getEntitiesInternal() { return _entities; };
  std::vector<System> &getSystemsInternal() { return _systems; };
  std::atomic<bool> &isPausedInternal() { return _isPaused; };

  std::vector<Entity *> resolveEntitiesInternal(const std::string &prefix);
  std::vector<Rect> getRectsSnapshot();

private:
  explicit GameLoop();
  std::mutex _mutex;
  std::map<std::string, Entity> _entities;
  std::vector<System> _systems{};
  std::atomic<bool> _isRunning{true};
  std::atomic<bool> _isPaused{true};
  GameStats _gameStats;
  std::unique_ptr<std::thread> _gameThread;

  static constexpr double TARGET_DELTA_TIME = 1.0 / 60.0;

  void runGameLoop();
  void runSystems();
  void update(double deltaTime);
  void updateStats(double deltaTime);
  void updateEntities(double deltaTime);
};
} // namespace margelo::nitro::rngine
