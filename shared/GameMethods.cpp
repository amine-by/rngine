#include "GameMethods.hpp"
#include "GameLoop.hpp"
#include <android/log.h>

namespace margelo::nitro::rngine {
void GameMethods::initialize(bool isPaused,
                             const std::vector<Entity> &entities) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  instance.isPausedInternal().store(isPaused);
  instance.getEntitiesInternal() = entities;
  __android_log_print(
      ANDROID_LOG_INFO, "GameMethods", "Initialized %zu entities. Status: %s",
      instance.getEntitiesInternal().size(), isPaused ? "PAUSED" : "RUNNING");
}

void GameMethods::pause() {
  auto &isPaused = GameLoop::getInstance().isPausedInternal();
  if (isPaused) {
    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "Already paused, ignoring");
    return;
  }

  __android_log_print(ANDROID_LOG_INFO, "GameMethods", "Pause");
  isPaused.store(true);
}

void GameMethods::resume() {
  auto &isPaused = GameLoop::getInstance().isPausedInternal();
  if (!isPaused) {
    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "Already running, ignoring");
    return;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods", "Resume");
  isPaused.store(false);
}

void GameMethods::spawn(const Entity &entity) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  if (instance.findEntityInternal(entity.id) != nullptr) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "spawn: entity already exists for id=%s",
                        entity.id.c_str());
    return;
  }
  instance.getEntitiesInternal().push_back(entity);
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "spawn: spawned entity id=%s", entity.id.c_str());
}

void GameMethods::despawn(const std::string &id) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &entities = instance.getEntitiesInternal();
  auto it = std::find_if(entities.begin(), entities.end(),
                         [&id](const Entity &e) { return e.id == id; });
  if (it == entities.end()) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "despawn: entity not found for id=%s", id.c_str());
    return;
  }
  entities.erase(it);
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "despawn: despawned entity id=%s", id.c_str());
}

void GameMethods::setP(const std::string &id, double px, double py) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());

  auto *entity = instance.findEntityInternal(id);
  if (entity == nullptr) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "setP: entity not found for id=%s", id.c_str());
    return;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setP: entity found, setting px=%f py=%f", px, py);
  entity->px = px;
  entity->py = py;
}

void GameMethods::setV(const std::string &id, double vx, double vy) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto *entity = instance.findEntityInternal(id);
  if (entity == nullptr) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "setV: entity not found for id=%s", id.c_str());
    return;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setV: entity found, setting vx=%f vy=%f", vx, vy);
  entity->vx = vx;
  entity->vy = vy;
}
} // namespace margelo::nitro::rngine
