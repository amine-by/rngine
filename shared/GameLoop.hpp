#pragma once

#include "Entity.hpp"
#include "GameStats.hpp"
#include "Screen.hpp"
#include "System.hpp"
#include "World.hpp"
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

  std::mutex &getWorldMutexInternal() { return _worldMutex; };
  std::mutex &getMutexInternal() { return _mutex; };
  std::map<string, Entity> &getEntitiesInternal() { return _entities; };
  std::vector<System> &getSystemsInternal() { return _systems; };
  std::atomic<bool> &getIsPausedInternal() { return _isPaused; };
  World &getWorldInternal() { return _world; };
  Screen &getScreenInternal() { return _screen; };

  bool entityIdMatches(const std::string &entityId, const std::string &prefix);
  std::vector<Entity *> resolveEntitiesInternal(const std::string &prefix);

private:
  explicit GameLoop();
  std::mutex _worldMutex;
  std::mutex _mutex;
  std::map<std::string, Entity> _entities;
  std::vector<Collision> _collisions{};
  std::vector<System> _systems{};
  std::atomic<bool> _isRunning{true};
  std::atomic<bool> _isPaused{true};
  World _world;
  Screen _screen;
  GameStats _gameStats;
  std::unique_ptr<std::thread> _gameThread;

  void runGameLoop();
  void runSystems();
  void computeCollisions();
  void resolveCollisions();
  void update(double deltaTime);
  void updateStats(double deltaTime);
  void updateScreen(double deltaTime);
  void updateEntities(double deltaTime);
};
} // namespace margelo::nitro::rngine
