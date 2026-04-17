#include "GameLoop.hpp"
#include <jni.h>

extern "C" {

JNIEXPORT void JNICALL Java_com_margelo_nitro_rngine_GameEngine_initializeLoop(
    JNIEnv *env, jobject, jobject byteBuffer, jint count) {
  auto *data = static_cast<uint8_t *>(env->GetDirectBufferAddress(byteBuffer));

  std::vector<rngine::Entity> entities;
  entities.reserve(count);

  for (int i = 0; i < count; i++) {
    rngine::Entity entity;
    memcpy(entity.id, data, rngine::ENTITY_ID_SIZE);
    data += rngine::ENTITY_ID_SIZE;
    memcpy(&entity.x, data, sizeof(double));
    data += sizeof(double);
    memcpy(&entity.y, data, sizeof(double));
    data += sizeof(double);
    memcpy(&entity.width, data, sizeof(double));
    data += sizeof(double);
    memcpy(&entity.height, data, sizeof(double));
    data += sizeof(double);
    memcpy(&entity.xv, data, sizeof(double));
    data += sizeof(double);
    memcpy(&entity.yv, data, sizeof(double));
    data += sizeof(double);
    entities.push_back(entity);
  }

  rngine::GameLoop::getInstance().initialize(std::move(entities));
}

JNIEXPORT void JNICALL
Java_com_margelo_nitro_rngine_GameEngine_pauseLoop(JNIEnv *, jobject) {
  rngine::GameLoop::getInstance().pause();
}

JNIEXPORT void JNICALL
Java_com_margelo_nitro_rngine_GameEngine_resumeLoop(JNIEnv *, jobject) {
  rngine::GameLoop::getInstance().resume();
}

JNIEXPORT jobject JNICALL
Java_com_margelo_nitro_rngine_GameView_getEntitiesSnapshot(JNIEnv *env,
                                                           jobject) {
  auto snapshot = rngine::GameLoop::getInstance().getEntitiesSnapshot();
  const size_t size = snapshot.size() * rngine::ENTITY_BUFFER_SIZE;

  static std::vector<uint8_t> buffer;
  buffer.resize(size);

  uint8_t *data = buffer.data();

  for (const auto &entity : snapshot) {
    memcpy(data, entity.id, rngine::ENTITY_ID_SIZE);
    data += rngine::ENTITY_ID_SIZE;
    memcpy(data, &entity.x, sizeof(double));
    data += sizeof(double);
    memcpy(data, &entity.y, sizeof(double));
    data += sizeof(double);
    memcpy(data, &entity.width, sizeof(double));
    data += sizeof(double);
    memcpy(data, &entity.height, sizeof(double));
    data += sizeof(double);
    memcpy(data, &entity.xv, sizeof(double));
    data += sizeof(double);
    memcpy(data, &entity.yv, sizeof(double));
    data += sizeof(double);
  }

  return env->NewDirectByteBuffer(buffer.data(), (jlong)size);
}
} // extern "C"
