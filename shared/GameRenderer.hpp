#pragma once
#include "Entity.hpp"
#include "Screen.hpp"
#include "include/core/SkSurface.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "modules/skottie/include/Skottie.h"
#include "modules/svg/include/SkSVGDOM.h"
#include <EGL/egl.h>
#include <android/native_window.h>
#include <cstddef>

namespace margelo::nitro::rngine {
class GameRenderer {
public:
  static GameRenderer &getInstance();

  GameRenderer(const GameRenderer &) = delete;
  GameRenderer &operator=(const GameRenderer &) = delete;
  GameRenderer(GameRenderer &&) = delete;
  GameRenderer &operator=(GameRenderer &&) = delete;

  ~GameRenderer();

  std::map<double, sk_sp<SkSVGDOM>> &getSvgCacheInternal() {
    return _svgCache;
  };
  std::map<double, sk_sp<skottie::Animation>> &getLottieCacheInternal() {
    return _lottieCache;
  };

  void onSurfaceCreated(ANativeWindow *window);
  void onSurfaceChanged(int width, int height);
  void onSurfaceDestroyed();

  void render(const Screen &screen,
              const std::map<std::string, Entity> &entities);

private:
  explicit GameRenderer();

  ANativeWindow *_nativeWindow{nullptr};

  EGLDisplay _eglDisplay{EGL_NO_DISPLAY};
  EGLSurface _eglSurface{EGL_NO_SURFACE};
  EGLContext _eglContext{EGL_NO_CONTEXT};
  EGLConfig _eglConfig{nullptr};

  sk_sp<GrDirectContext> _grContext;
  sk_sp<SkSurface> _surface;

  int _width{0};
  int _height{0};
  bool _initialized{false};

  std::map<double, sk_sp<SkSVGDOM>> _svgCache;
  std::map<double, sk_sp<skottie::Animation>> _lottieCache;

  bool isEntityVisible(const Entity &entity, float screenWidth,
                       float screenHeight);

  bool initializeEGL();
  bool createEGLSurface();
  bool createSkSurface();
};
} // namespace margelo::nitro::rngine
