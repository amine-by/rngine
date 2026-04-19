#pragma once

#include "Entity.hpp"
#include "GameStats.hpp"
#include <atomic>
#include <memory>
#include <thread>

namespace rngine {
class GameLoop {
public:
  static GameLoop &getInstance();

  GameLoop(const GameLoop &) = delete;
  GameLoop &operator=(const GameLoop &) = delete;
  GameLoop(GameLoop &&) = delete;
  GameLoop &operator=(GameLoop &&) = delete;

  ~GameLoop();

  void initialize(std::vector<Entity> entities);
  void pause();
  void resume();
  std::vector<Entity> getEntitiesSnapshot();
  Entity *getEntityById(const std::string &id);

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
} // namespace rngine
