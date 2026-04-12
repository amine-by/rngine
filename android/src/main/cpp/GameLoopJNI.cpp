#include "GameLoop.hpp"
#include <jni.h>

// one global instance — or you can manage a pointer per Kotlin object
static rngine::GameLoop *instance = nullptr;

extern "C" {

JNIEXPORT void JNICALL Java_com_margelo_nitro_rngine_GameEngine_createLoop(JNIEnv *,
                                                             jobject) {
  instance = new rngine::GameLoop();
}

JNIEXPORT void JNICALL Java_com_margelo_nitro_rngine_GameEngine_destroyLoop(JNIEnv *,
                                                              jobject) {
  delete instance;
  instance = nullptr;
}

JNIEXPORT void JNICALL Java_com_margelo_nitro_rngine_GameEngine_pauseLoop(JNIEnv *, jobject) {
  if (instance)
    instance->pause();
}

JNIEXPORT void JNICALL Java_com_margelo_nitro_rngine_GameEngine_resumeLoop(JNIEnv *,
                                                             jobject) {
  if (instance)
    instance->resume();
}
} // extern "C"
