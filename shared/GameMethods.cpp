#include "GameMethods.hpp"
#include "EntityInitializer.hpp"
#include "EntityUpdate.hpp"
#include "GameLoop.hpp"
#include "GameRenderer.hpp"
#include "ObjectFit.hpp"
#include "Repeat.hpp"
#include "ScreenInitializer.hpp"
#include "WorldInitializer.hpp"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include <android/log.h>
#include <fbjni/fbjni.h>

namespace margelo::nitro::rngine {
void GameMethods::setWorld(const WorldInitializer &worldInitializer) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getWorldMutexInternal());
  auto &worldInternal = instance.getWorldInternal();

  worldInternal.tickRate = worldInitializer.tickRate;
  worldInternal.gx = worldInitializer.gx.value_or(0.0);
  worldInternal.gy = worldInitializer.gy.value_or(0.0);

  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setWorld: tickRate=%.1f", worldInternal.tickRate);
}

void GameMethods::setScreen(const ScreenInitializer &screenInitializer) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &screenInternal = instance.getScreenInternal();

  screenInternal.px = screenInitializer.px;
  screenInternal.py = screenInitializer.py;
  screenInternal.width = screenInitializer.width;
  screenInternal.height = screenInitializer.height;
  screenInternal.color = screenInitializer.color.value_or("transparent");
  screenInternal.asset = screenInitializer.asset.value_or(0.0);
  screenInternal.flipH = screenInitializer.flipH.value_or(false);
  screenInternal.flipV = screenInitializer.flipV.value_or(false);
  screenInternal.objectFit =
      screenInitializer.objectFit.value_or(ObjectFit::FILL);
  screenInternal.clip = screenInitializer.clip.value_or(false);
  screenInternal.repeat = screenInitializer.repeat.value_or(Repeat::NO_REPEAT);
  screenInternal.progress = screenInitializer.progress.value_or(0.0);
  screenInternal.speed = screenInitializer.speed.value_or(1.0);
  screenInternal.loop = screenInitializer.loop.value_or(false);

  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setScreen: width=%.0f height=%.0f", screenInternal.width,
                      screenInternal.height);
}

void GameMethods::setEntities(
    const std::vector<EntityInitializer> &entitiesInitializer) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &entitiesInternal = instance.getEntitiesInternal();

  entitiesInternal.clear();
  for (const auto &entityInitializer : entitiesInitializer) {
    auto &entityInternal = entitiesInternal[entityInitializer.id];

    entityInternal.id = entityInitializer.id;
    entityInternal.px = entityInitializer.px;
    entityInternal.py = entityInitializer.py;
    entityInternal.shape = entityInitializer.shape;
    entityInternal.angle = entityInitializer.angle.value_or(0.0);
    entityInternal.isSensor = entityInitializer.isSensor.value_or(false);
    entityInternal.color = entityInitializer.color.value_or("transparent");
    entityInternal.asset = entityInitializer.asset.value_or(0.0);
    entityInternal.flipH = entityInitializer.flipH.value_or(false);
    entityInternal.flipV = entityInitializer.flipV.value_or(false);
    entityInternal.objectFit =
        entityInitializer.objectFit.value_or(ObjectFit::FILL);
    entityInternal.clip = entityInitializer.clip.value_or(false);
    entityInternal.repeat =
        entityInitializer.repeat.value_or(Repeat::NO_REPEAT);
    entityInternal.progress = entityInitializer.progress.value_or(0.0);
    entityInternal.speed = entityInitializer.speed.value_or(1.0);
    entityInternal.loop = entityInitializer.loop.value_or(false);
    entityInternal.vx = entityInitializer.vx.value_or(0.0);
    entityInternal.vy = entityInitializer.vy.value_or(0.0);
    entityInternal.ax = entityInitializer.ax.value_or(0.0);
    entityInternal.ay = entityInitializer.ay.value_or(0.0);
    entityInternal.mass = entityInitializer.mass.value_or(0.0);
    entityInternal.restitution = entityInitializer.restitution.value_or(0.0);
  }

  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setEntities: loaded %zu entities",
                      entitiesInternal.size());
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

void GameMethods::spawn(
    const std::vector<EntityInitializer> &entitiesInitializer) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &entitiesInternal = instance.getEntitiesInternal();

  for (const auto &entityInitializer : entitiesInitializer) {
    if (entitiesInternal.count(entityInitializer.id)) {
      __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                          "spawn: entity already exists for id=%s",
                          entityInitializer.id.c_str());
      continue;
    }

    auto &entityInternal = entitiesInternal[entityInitializer.id];

    entityInternal.id = entityInitializer.id;
    entityInternal.px = entityInitializer.px;
    entityInternal.py = entityInitializer.py;
    entityInternal.shape = entityInitializer.shape;
    entityInternal.angle = entityInitializer.angle.value_or(0.0);
    entityInternal.isSensor = entityInitializer.isSensor.value_or(false);
    entityInternal.color = entityInitializer.color.value_or("transparent");
    entityInternal.asset = entityInitializer.asset.value_or(0.0);
    entityInternal.flipH = entityInitializer.flipH.value_or(false);
    entityInternal.flipV = entityInitializer.flipV.value_or(false);
    entityInternal.objectFit =
        entityInitializer.objectFit.value_or(ObjectFit::FILL);
    entityInternal.clip = entityInitializer.clip.value_or(false);
    entityInternal.repeat =
        entityInitializer.repeat.value_or(Repeat::NO_REPEAT);
    entityInternal.progress = entityInitializer.progress.value_or(0.0);
    entityInternal.speed = entityInitializer.speed.value_or(1.0);
    entityInternal.loop = entityInitializer.loop.value_or(false);
    entityInternal.vx = entityInitializer.vx.value_or(0.0);
    entityInternal.vy = entityInitializer.vy.value_or(0.0);
    entityInternal.ax = entityInitializer.ax.value_or(0.0);
    entityInternal.ay = entityInitializer.ay.value_or(0.0);
    entityInternal.mass = entityInitializer.mass.value_or(0.0);
    entityInternal.restitution = entityInitializer.restitution.value_or(0.0);

    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "spawn: spawned entity id=%s",
                        entityInternal.id.c_str());
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

static void patch(const auto &field, auto &target) {
  if (field.has_value()) {
    target = field.value();
  }
}

void GameMethods::updateEntities(
    const std::vector<EntityUpdate> &entityUpdates) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  for (const auto &entityUpdate : entityUpdates) {
    auto resolvedEntitiesInternals =
        instance.resolveEntitiesInternal(entityUpdate.id);
    if (resolvedEntitiesInternals.empty()) {
      __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                          "entityUpdate: no entity found for id=%s",
                          entityUpdate.id.c_str());
      continue;
    }

    for (auto *entity : resolvedEntitiesInternals) {
      patch(entityUpdate.px, entity->px);
      patch(entityUpdate.py, entity->py);
      patch(entityUpdate.shape, entity->shape);
      patch(entityUpdate.angle, entity->angle);
      patch(entityUpdate.isSensor, entity->isSensor);
      patch(entityUpdate.asset, entity->asset);
      patch(entityUpdate.progress, entity->progress);
      patch(entityUpdate.speed, entity->speed);
      patch(entityUpdate.loop, entity->loop);
      patch(entityUpdate.color, entity->color);
      patch(entityUpdate.flipH, entity->flipH);
      patch(entityUpdate.flipV, entity->flipV);
      patch(entityUpdate.objectFit, entity->objectFit);
      patch(entityUpdate.clip, entity->clip);
      patch(entityUpdate.repeat, entity->repeat);
      patch(entityUpdate.vx, entity->vx);
      patch(entityUpdate.vy, entity->vy);
      patch(entityUpdate.ax, entity->ax);
      patch(entityUpdate.ay, entity->ay);
      patch(entityUpdate.mass, entity->mass);
      patch(entityUpdate.restitution, entity->restitution);
    }

    __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                        "entityUpdateEntities: patched %zu entities for id=%s",
                        resolvedEntitiesInternals.size(),
                        entityUpdate.id.c_str());
  }
}

void GameMethods::updateScreen(const ScreenUpdate &screenUpdate) {
  auto &instance = GameLoop::getInstance();
  std::lock_guard<std::mutex> lock(instance.getMutexInternal());
  auto &screen = instance.getScreenInternal();

  patch(screenUpdate.px, screen.px);
  patch(screenUpdate.py, screen.py);
  patch(screenUpdate.width, screen.width);
  patch(screenUpdate.height, screen.height);
  patch(screenUpdate.asset, screen.asset);
  patch(screenUpdate.progress, screen.progress);
  patch(screenUpdate.speed, screen.speed);
  patch(screenUpdate.loop, screen.loop);
  patch(screenUpdate.color, screen.color);
  patch(screenUpdate.flipH, screen.flipH);
  patch(screenUpdate.flipV, screen.flipV);
  patch(screenUpdate.objectFit, screen.objectFit);
  patch(screenUpdate.clip, screen.clip);
  patch(screenUpdate.repeat, screen.repeat);

  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "updateScreen: patched screen");
}

bool GameMethods::isAssetLoaded(double id) {
  auto &gameRenderer = GameRenderer::getInstance();

  if (id > 0.0) {
    auto &imageCache = gameRenderer.getImageCacheInternal();
    auto it = imageCache.find(id);
    return it != imageCache.end();
  }

  if (id < 0.0) {
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
