#include "GameMethods.hpp"
#include "EntityUpdate.hpp"
#include "GameLoop.hpp"
#include <android/log.h>

namespace margelo::nitro::rngine {
void GameMethods::setTickRate(double tickRate) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  instance.setTickRate(tickRate);
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setTickRate: tickRate=%.1f", tickRate);
}

void GameMethods::setScreen(const Screen &screen) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  instance.getScreenInternal() = screen;
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setWorld: width=%.0f height=%.0f", screen.width,
                      screen.height);
}

void GameMethods::setEntities(const std::vector<Entity> &entities) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &entitiesInternal = instance.getEntitiesInternal();
  for (auto &entity : entities) {
    entitiesInternal[entity.id] = entity;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setEntities: loaded %zu entities", entities.size());
}

void GameMethods::setSystems(const std::vector<System> &systems) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  instance.getSystemsInternal() = systems;
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setSystems: loaded %zu systems", systems.size());
}

void GameMethods::pause() {
  auto &isPaused = GameLoop::getInstance().getIsPausedInternal();
  if (isPaused) {
    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "Already paused, ignoring");
    return;
  }

  __android_log_print(ANDROID_LOG_INFO, "GameMethods", "Pause");
  isPaused.store(true);
}

void GameMethods::resume() {
  auto &isPaused = GameLoop::getInstance().getIsPausedInternal();
  if (!isPaused) {
    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "Already running, ignoring");
    return;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods", "Resume");
  isPaused.store(false);
}

void GameMethods::spawn(const std::vector<Entity> &entities) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &entitiesInternal = instance.getEntitiesInternal();

  for (const auto &entity : entities) {
    if (entitiesInternal.count(entity.id)) {
      __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                          "spawn: entity already exists for id=%s",
                          entity.id.c_str());
      continue;
    }
    entitiesInternal[entity.id] = entity;
    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "spawn: spawned entity id=%s", entity.id.c_str());
  }
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

void GameMethods::update(const std::vector<EntityUpdate> &updates) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  for (const auto &update : updates) {
    auto resolvedEntitiesInternals =
        instance.resolveEntitiesInternal(update.id);
    if (resolvedEntitiesInternals.empty()) {
      __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                          "update: no entity found for id=%s",
                          update.id.c_str());
      continue;
    }

    for (auto *entity : resolvedEntitiesInternals) {
      if (update.px.has_value()) {
        entity->px = update.px.value();
      }
      if (update.py.has_value()) {
        entity->py = update.py.value();
      }
      if (update.width.has_value()) {
        entity->width = update.width.value();
      }
      if (update.height.has_value()) {
        entity->height = update.height.value();
      }
      if (update.asset.has_value()) {
        entity->asset = update.asset.value();
      }
      if (update.progress.has_value()) {
        entity->progress = update.progress.value();
      }
      if (update.color.has_value()) {
        entity->color = update.color.value();
      }
      if (update.vx.has_value()) {
        entity->vx = update.vx.value();
      }
      if (update.vy.has_value()) {
        entity->vy = update.vy.value();
      }
    }

    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "update: patched %zu entities for id=%s",
                        resolvedEntitiesInternals.size(), update.id.c_str());
  }
}
} // namespace margelo::nitro::rngine
