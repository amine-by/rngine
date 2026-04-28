#include "GameMethods.hpp"
#include "GameLoop.hpp"
#include <android/log.h>

namespace margelo::nitro::rngine {
void GameMethods::initialize(bool isPaused, const std::vector<Entity> &entities,
                             const std::vector<System> &systems) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &entitiesInternal = instance.getEntitiesInternal();

  instance.isPausedInternal().store(isPaused);

  for (auto &entity : entities) {
    entitiesInternal[entity.id] = entity;
  }

  instance.getSystemsInternal() = systems;
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "Initialized %zu entities and %zu systems. Status: %s",
                      instance.getEntitiesInternal().size(),
                      instance.getSystemsInternal().size(),
                      isPaused ? "PAUSED" : "RUNNING");
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
  auto &entitiesInternal = instance.getEntitiesInternal();
  if (entitiesInternal.count(entity.id)) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "spawn: entity already exists for id=%s",
                        entity.id.c_str());
    return;
  }
  entitiesInternal[entity.id] = entity;
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "spawn: spawned entity id=%s", entity.id.c_str());
}

void GameMethods::despawn(const std::string &id) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &entities = instance.getEntitiesInternal();

  auto resolved = instance.resolveEntitiesInternal(id);
  if (resolved.empty()) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "despawn: no entities found for id=%s", id.c_str());
    return;
  }

  for (auto *entity : resolved) {
    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "despawn: despawned entity id=%s", entity->id.c_str());
    entities.erase(entity->id);
  }
}

void GameMethods::setP(const std::string &id, double px, double py) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &entitiesInternal = instance.getEntitiesInternal();
  auto it = entitiesInternal.find(id);

  if (it == entitiesInternal.end()) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "setP: entity not found for id=%s", id.c_str());
    return;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setP: entity found, setting px=%f py=%f", px, py);
  it->second.px = px;
  it->second.py = py;
}

void GameMethods::setV(const std::string &id, double vx, double vy) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto resolvedEntitiesInternals = instance.resolveEntitiesInternal(id);

  if (resolvedEntitiesInternals.empty()) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "setV: entity not found for id=%s", id.c_str());
    return;
  }
  for (auto *entity : resolvedEntitiesInternals) {
    entity->vx = vx;
    entity->vy = vy;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setV: set vx=%f vy=%f for %zu entities matching id=%s",
                      vx, vy, resolvedEntitiesInternals.size(), id.c_str());
}
} // namespace margelo::nitro::rngine
