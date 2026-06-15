#pragma once

#include "Entity.hpp"
#include "GameStats.hpp"
#include "Rect.hpp"
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
  std::map<string, Entity> &getEntitiesInternal() { return _entities; };
  std::vector<System> &getSystemsInternal() { return _systems; };
  std::atomic<bool> &getIsPausedInternal() { return _isPaused; };
  Screen &getScreenInternal() { return _screen; };

  std::vector<Entity *> resolveEntitiesInternal(const std::string &prefix);
  void registerLottieDuration(double, double);
  std::vector<Rect> getRectsSnapshot();

  void setTickRate(double tickRate) { _tickRate = tickRate; };

private:
  explicit GameLoop();
  std::mutex _mutex;
  std::map<std::string, Entity> _entities;
  std::map<double, double> _lottieDurations;
  std::vector<System> _systems{};
  std::atomic<bool> _isRunning{true};
  std::atomic<bool> _isPaused{true};
  double _tickRate{60.0};
  Screen _screen;
  GameStats _gameStats;
  std::unique_ptr<std::thread> _gameThread;

  void runGameLoop();
  void runSystems();
  void update(double deltaTime);
  void updateStats(double deltaTime);
  void updateEntities(double deltaTime);
};
} // namespace margelo::nitro::rngine
