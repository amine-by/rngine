#pragma once

#include "Entity.hpp"
#include "GameStats.hpp"
#include "Screen.hpp"
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
  std::mutex &getSnapshotMutexInternal() { return _snapshotMutex; };
  std::map<string, Entity> &getEntitiesInternal() { return _entities; };
  std::map<string, Entity> &getEntitiesSnapshotInternal() {
    return _entitiesSnapshot;
  };
  std::vector<System> &getSystemsInternal() { return _systems; };
  std::atomic<bool> &getIsPausedInternal() { return _isPaused; };
  Screen &getScreenInternal() { return _screen; };
  Screen &getScreenSnapshotInternal() { return _screenSnapshot; };

  std::vector<Entity *> resolveEntitiesInternal(const std::string &prefix);
  void registerLottieDuration(double, double);

  void setTickRate(double tickRate) { _tickRate.store(tickRate); };

private:
  explicit GameLoop();
  std::mutex _mutex;
  std::mutex _snapshotMutex;
  std::map<std::string, Entity> _entities;
  std::map<std::string, Entity> _entitiesSnapshot;
  std::map<double, double> _lottieDurations;
  std::vector<System> _systems{};
  std::atomic<bool> _isRunning{true};
  std::atomic<bool> _isPaused{true};
  std::atomic<double> _tickRate{60.0};
  Screen _screen;
  Screen _screenSnapshot;
  GameStats _gameStats;
  std::unique_ptr<std::thread> _gameThread;

  void runGameLoop();
  void runSystems();
  void captureSnapshot();
  void update(double deltaTime);
  void updateStats(double deltaTime);
  void updateEntities(double deltaTime);
};
} // namespace margelo::nitro::rngine
