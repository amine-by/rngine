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
    memcpy(data, &rect.left, sizeof(double));
    data += sizeof(double);
    memcpy(data, &rect.right, sizeof(double));
    data += sizeof(double);
    memcpy(data, &rect.top, sizeof(double));
    data += sizeof(double);
    memcpy(data, &rect.bottom, sizeof(double));
    data += sizeof(double);
  }

  return env->NewDirectByteBuffer(buffer.data(), (jlong)size);
}
} // extern "C"
