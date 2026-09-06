#include "GameRenderer.hpp"
#include <android/native_window_jni.h>
#include <jni.h>

extern "C" JNIEXPORT void JNICALL
Java_com_margelo_nitro_rngine_GameView_nativeOnSurfaceCreated(JNIEnv *env,
                                                              jobject instance,
                                                              jobject surface) {

  ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
  margelo::nitro::rngine::GameRenderer::getInstance().onSurfaceCreated(
      nativeWindow);
}

extern "C" JNIEXPORT void JNICALL
Java_com_margelo_nitro_rngine_GameView_nativeOnSurfaceChanged(JNIEnv *env,
                                                              jobject instance,
                                                              jint width,
                                                              jint height) {

  margelo::nitro::rngine::GameRenderer::getInstance().onSurfaceChanged(width,
                                                                       height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_margelo_nitro_rngine_GameView_nativeOnSurfaceDestroyed(
    JNIEnv *env, jobject instance) {
  margelo::nitro::rngine::GameRenderer::getInstance().onSurfaceDestroyed();
}
