#include "GameMethods.hpp"
#include "EntityUpdate.hpp"
#include "GameLoop.hpp"
#include "GameRenderer.hpp"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include <android/log.h>
#include <fbjni/fbjni.h>

namespace margelo::nitro::rngine {
void GameMethods::setWorld(const World &world) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getWorldMutexInternal());
  instance.getWorldInternal() = world;
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setWorld: tickRate=%.1f", world.tickRate);
}

void GameMethods::setScreen(const Screen &screen) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  instance.getScreenInternal() = screen;

  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setScreen: width=%.0f height=%.0f", screen.width,
                      screen.height);
}

void GameMethods::setEntities(const std::vector<Entity> &entities) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &entitiesInternal = instance.getEntitiesInternal();
  entitiesInternal.clear();
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
      if (update.shape.has_value()) {
        entity->shape = update.shape.value();
      }
      if (update.isSensor.has_value()) {
        entity->isSensor = update.isSensor.value();
      }
      if (update.asset.has_value()) {
        entity->asset = update.asset.value();
      }
      if (update.progress.has_value()) {
        entity->progress = update.progress.value();
      }
      if (update.speed.has_value()) {
        entity->speed = update.speed.value();
      }
      if (update.color.has_value()) {
        entity->color = update.color.value();
      }
      if (update.flipH.has_value()) {
        entity->flipH = update.flipH.value();
      }
      if (update.flipV.has_value()) {
        entity->flipV = update.flipV.value();
      }
      if (update.vx.has_value()) {
        entity->vx = update.vx.value();
      }
      if (update.vy.has_value()) {
        entity->vy = update.vy.value();
      }
      if (update.ax.has_value()) {
        entity->ax = update.ax.value();
      }
      if (update.ay.has_value()) {
        entity->ay = update.ay.value();
      }
      if (update.mass.has_value()) {
        entity->mass = update.mass.value();
      }
    }

    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "update: patched %zu entities for id=%s",
                        resolvedEntitiesInternals.size(), update.id.c_str());
  }
}

bool GameMethods::isAssetLoaded(double id) {
  auto &gameRenderer = GameRenderer::getInstance();

  if (id > 0) {
    auto &imageCache = gameRenderer.getImageCacheInternal();
    auto it = imageCache.find(id);
    return it != imageCache.end();
  }

  if (id < 0) {
    auto &lottieCache = gameRenderer.getLottieCacheInternal();
    auto it = lottieCache.find(id);
    return it != lottieCache.end();
  }

  return false;
};

std::shared_ptr<Promise<bool>>
GameMethods::loadLottie(double id, const std::string &jsonStr) {
  return Promise<bool>::async([=]() -> bool {
    auto resourceProvider = skresources::DataURIResourceProviderProxy::Make(
        nullptr, skresources::ImageDecodeStrategy::kLazyDecode);

    auto animation = skottie::Animation::Builder()
                         .setResourceProvider(resourceProvider)
                         .make(jsonStr.data(), jsonStr.size());

    if (!animation) {
      return false;
    }

    auto &gameRenderer = GameRenderer::getInstance();
    auto &lottieCache = gameRenderer.getLottieCacheInternal();

    lottieCache[id] = std::move(animation);

    return true;
  });
};

std::shared_ptr<Promise<bool>>
GameMethods::loadImage(double id, const std::string &imageUri) {
  return Promise<bool>::async([=]() -> bool {
    __android_log_print(ANDROID_LOG_DEBUG, "GameMethods",
                        "loadImage: start id=%.0f uri=%s", id,
                        imageUri.c_str());
    std::string imageStr;

    using namespace facebook::jni;

    try {
      ThreadScope::WithClassLoader([&] {
        static const auto helperClass =
            findClassStatic("com/margelo/nitro/rngine/ImageLoaderHelper");

        local_ref<JArrayByte> byteArray;

        if (imageUri.rfind("http://", 0) == 0 ||
            imageUri.rfind("https://", 0) == 0) {
          static const auto method =
              helperClass->getStaticMethod<JArrayByte(local_ref<JString>)>(
                  "loadFromUrl");
          byteArray = method(helperClass, make_jstring(imageUri));
        } else {
          static const auto method =
              helperClass->getStaticMethod<JArrayByte(local_ref<JString>)>(
                  "loadFromResource");
          byteArray = method(helperClass, make_jstring(imageUri));
        }

        if (!byteArray) {
          throw std::runtime_error("Failed to load image bytes for: " +
                                   imageUri);
        }

        size_t len = byteArray->size();
        imageStr.resize(len);
        byteArray->getRegion(0, len,
                             reinterpret_cast<int8_t *>(imageStr.data()));
      });
    } catch (const std::exception &e) {
      __android_log_print(ANDROID_LOG_ERROR, "GameMethods",
                          "loadImage: JNI fetch failed id=%.0f: %s", id,
                          e.what());
      return false;
    }

    __android_log_print(ANDROID_LOG_DEBUG, "GameMethods",
                        "loadImage: fetched %zu bytes id=%.0f", imageStr.size(),
                        id);

    auto &gameRenderer = GameRenderer::getInstance();
    auto &imageCache = gameRenderer.getImageCacheInternal();

    auto stream = SkMemoryStream::MakeDirect(imageStr.data(), imageStr.size());
    auto svgDom = SkSVGDOM::MakeFromStream(*stream);

    if (svgDom) {
      __android_log_print(ANDROID_LOG_DEBUG, "GameMethods",
                          "loadImage: parsed as SVG id=%.0f", id);
      imageCache[id] = std::move(svgDom);
      return true;
    }

    auto data = SkData::MakeWithCopy(imageStr.data(), imageStr.size());
    auto raster = SkImages::DeferredFromEncodedData(std::move(data));

    if (raster) {
      __android_log_print(ANDROID_LOG_DEBUG, "GameMethods",
                          "loadImage: parsed as raster id=%.0f", id);
      imageCache[id] = std::move(raster);
      return true;
    }

    __android_log_print(ANDROID_LOG_ERROR, "GameMethods",
                        "loadImage: both SVG parse and raster decode failed "
                        "id=%.0f",
                        id);

    return false;
  });
};
} // namespace margelo::nitro::rngine
