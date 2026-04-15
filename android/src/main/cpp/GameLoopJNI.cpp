#include "GameLoop.hpp"
#include <jni.h>

// one global instance — or you can manage a pointer per Kotlin object
static rngine::GameLoop *instance = nullptr;

extern "C" {

JNIEXPORT void JNICALL Java_com_margelo_nitro_rngine_GameEngine_createLoop(
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
    entities.push_back(entity);
  }

  instance = new rngine::GameLoop(std::move(entities));
}

JNIEXPORT void JNICALL
Java_com_margelo_nitro_rngine_GameEngine_destroyLoop(JNIEnv *, jobject) {
  delete instance;
  instance = nullptr;
}

JNIEXPORT void JNICALL
Java_com_margelo_nitro_rngine_GameEngine_pauseLoop(JNIEnv *, jobject) {
  if (instance)
    instance->pause();
}

JNIEXPORT void JNICALL
Java_com_margelo_nitro_rngine_GameEngine_resumeLoop(JNIEnv *, jobject) {
  if (instance)
    instance->resume();
}

JNIEXPORT jobject JNICALL
Java_com_margelo_nitro_rngine_GameView_getEntitiesSnapshot(JNIEnv *env,
                                                           jobject) {
  if (!instance)
    return env->NewDirectByteBuffer(nullptr, 0);

  auto snapshot = instance->getEntitiesSnapshot();
  const size_t size = snapshot.size() * rngine::ENTITY_BUFFER_SIZE;

  static std::vector<uint8_t> buffer;
  buffer.resize(size);

  uint8_t *ptr = buffer.data();

  for (const auto &e : snapshot) {
    memcpy(ptr, e.id, rngine::ENTITY_ID_SIZE);
    ptr += rngine::ENTITY_ID_SIZE;
    memcpy(ptr, &e.x, sizeof(double));
    ptr += sizeof(double);
    memcpy(ptr, &e.y, sizeof(double));
    ptr += sizeof(double);
    memcpy(ptr, &e.width, sizeof(double));
    ptr += sizeof(double);
    memcpy(ptr, &e.height, sizeof(double));
    ptr += sizeof(double);
  }

  return env->NewDirectByteBuffer(buffer.data(), (jlong)size);
}
} // extern "C"
