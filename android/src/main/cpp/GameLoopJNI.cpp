#include "ColorUtils.hpp"
#include "GameLoop.hpp"
#include <cstdint>
#include <jni.h>
#include <mutex>
#include <variant>

extern "C" {
JNIEXPORT jbyteArray JNICALL
Java_com_margelo_nitro_rngine_GameView_getSnapshot(JNIEnv *env, jobject) {
  auto &gameLoop = margelo::nitro::rngine::GameLoop::getInstance();
  auto &snapshotMutex = gameLoop.getSnapshotMutexInternal();

  std::lock_guard<std::mutex> snapshotLock(snapshotMutex);
  auto &screenSnapshot = gameLoop.getScreenSnapshotInternal();
  auto &entitiesSnapshot = gameLoop.getEntitiesSnapshotInternal();

  std::vector<uint8_t> buffer;

  auto writeFloat = [&](float v) {
    const auto *bytes = reinterpret_cast<const uint8_t *>(&v);
    buffer.insert(buffer.end(), bytes, bytes + sizeof(float));
  };
  auto writeI32 = [&](int32_t v) {
    const auto *bytes = reinterpret_cast<const uint8_t *>(&v);
    buffer.insert(buffer.end(), bytes, bytes + sizeof(int32_t));
  };
  auto writeU32 = [&](uint32_t v) {
    const auto *bytes = reinterpret_cast<const uint8_t *>(&v);
    buffer.insert(buffer.end(), bytes, bytes + sizeof(uint32_t));
  };

  const auto screenSnapshotWidth = static_cast<float>(screenSnapshot.width);
  const auto screenSnapshotHeight = static_cast<float>(screenSnapshot.height);
  const auto screenSnapshotColor =
      margelo::nitro::rngine::parseHexColor(screenSnapshot.color);
  const auto screenSnapshotAsset =
      static_cast<int32_t>(screenSnapshot.asset.value_or(0));

  writeFloat(screenSnapshotWidth);
  writeFloat(screenSnapshotHeight);
  writeU32(screenSnapshotColor);
  writeI32(screenSnapshotAsset);

  if (screenSnapshotAsset < 0) {
    writeFloat(static_cast<float>(screenSnapshot.progress.value_or(0)));
  }

  for (const auto &[_, entitySnapshot] : entitiesSnapshot) {
    uint8_t shapeType{0};
    float entitySnapshotLeft{0}, entitySnapshotRight{0}, entitySnapshotTop{0},
        entitySnapshotBottom{0}, circleRadius{0};

    std::visit(
        [&](const auto &shape) {
          using T = std::decay_t<decltype(shape)>;
          if constexpr (std::is_same_v<T, margelo::nitro::rngine::Rect>) {
            shapeType = 0;
            entitySnapshotLeft =
                static_cast<float>(entitySnapshot.px - shape.width / 2.0);
            entitySnapshotRight =
                static_cast<float>(entitySnapshot.px + shape.width / 2.0);
            entitySnapshotTop =
                static_cast<float>(entitySnapshot.py - shape.height / 2.0);
            entitySnapshotBottom =
                static_cast<float>(entitySnapshot.py + shape.height / 2.0);
          } else {
            shapeType = 1;
            circleRadius = static_cast<float>(shape.radius);
            entitySnapshotLeft =
                static_cast<float>(entitySnapshot.px - shape.radius);
            entitySnapshotRight =
                static_cast<float>(entitySnapshot.px + shape.radius);
            entitySnapshotTop =
                static_cast<float>(entitySnapshot.py - shape.radius);
            entitySnapshotBottom =
                static_cast<float>(entitySnapshot.py + shape.radius);
          }
        },
        entitySnapshot.shape);

    if (entitySnapshotRight < 0 || entitySnapshotLeft > screenSnapshot.width ||
        entitySnapshotBottom < 0 || entitySnapshotTop > screenSnapshot.height) {
      continue;
    }

    const auto entitySnapshotColor =
        margelo::nitro::rngine::parseHexColor(entitySnapshot.color);
    const auto entitySnapshotAsset =
        static_cast<int32_t>(entitySnapshot.asset.value_or(0));

    buffer.push_back(shapeType);

    if (shapeType == 0) {
      writeFloat(entitySnapshotLeft);
      writeFloat(entitySnapshotRight);
      writeFloat(entitySnapshotTop);
      writeFloat(entitySnapshotBottom);
    } else {
      writeFloat(static_cast<float>(entitySnapshot.px));
      writeFloat(static_cast<float>(entitySnapshot.py));
      writeFloat(circleRadius);
    }

    writeU32(entitySnapshotColor);
    writeI32(entitySnapshotAsset);

    if (entitySnapshotAsset < 0) {
      writeFloat(static_cast<float>(entitySnapshot.progress.value_or(0)));
    }
  }

  auto bufferSize = static_cast<jsize>(buffer.size());
  jbyteArray result = env->NewByteArray(bufferSize);

  if (result == nullptr) {
    return nullptr;
  }

  env->SetByteArrayRegion(result, 0, bufferSize,
                          reinterpret_cast<const jbyte *>(buffer.data()));

  return result;
}

JNIEXPORT void JNICALL
Java_com_margelo_nitro_rngine_GameAssets_registerLottieDuration(
    JNIEnv *, jobject, jdouble id, jdouble duration) {
  margelo::nitro::rngine::GameLoop::getInstance().registerLottieDuration(
      id, duration);
}
} // extern "C"
