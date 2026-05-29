#include "GameLoop.hpp"
#include <jni.h>

extern "C" {
JNIEXPORT jobject JNICALL
Java_com_margelo_nitro_rngine_GameView_getRectsSnapshot(JNIEnv *env, jobject) {
  auto rects =
      margelo::nitro::rngine::GameLoop::getInstance().getRectsSnapshot();
  const size_t size = rects.size() * margelo::nitro::rngine::RECT_SIZE;

  static std::vector<uint8_t> buffer;
  buffer.resize(size);

  uint8_t *data = buffer.data();

  for (const auto &rect : rects) {
    memcpy(data, &rect.left, sizeof(float));
    data += sizeof(float);
    memcpy(data, &rect.right, sizeof(float));
    data += sizeof(float);
    memcpy(data, &rect.top, sizeof(float));
    data += sizeof(float);
    memcpy(data, &rect.bottom, sizeof(float));
    data += sizeof(float);
    memcpy(data, &rect.color, sizeof(uint32_t));
    data += sizeof(uint32_t);
    memcpy(data, &rect.asset, sizeof(uint32_t));
    data += sizeof(uint32_t);
  }

  return env->NewDirectByteBuffer(buffer.data(), (jlong)size);
}
} // extern "C"
