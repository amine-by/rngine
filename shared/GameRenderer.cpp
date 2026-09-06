#include "GameRenderer.hpp"
#include "ColorUtils.hpp"
#include "Rect.hpp"

#include <android/log.h>
#include <android/native_window.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkPaint.h"
#include "include/core/SkRect.h"
#include "include/core/SkSurface.h"

#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"

#include "include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <variant>

namespace margelo::nitro::rngine {

GameRenderer &GameRenderer::getInstance() {
  static GameRenderer instance;
  return instance;
}

GameRenderer::GameRenderer() {
  __android_log_print(ANDROID_LOG_INFO, "GameRenderer", "Constructor");
}

GameRenderer::~GameRenderer() {
  __android_log_print(ANDROID_LOG_INFO, "GameRenderer", "Destructor");
}

void GameRenderer::onSurfaceCreated(ANativeWindow *window) {
  __android_log_print(ANDROID_LOG_INFO, "GameRenderer", "onSurfaceCreated");

  _nativeWindow = window;

  if (!_initialized) {
    if (!initializeEGL()) {
      __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                          "onSurfaceCreated: EGL initialization failed");
      return;
    }
    _initialized = true;
  }

  if (!createEGLSurface()) {
    __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                        "onSurfaceCreated: EGL surface creation failed");
  }
}

void GameRenderer::onSurfaceChanged(int width, int height) {
  _width = width;
  _height = height;
  _surface = nullptr;

  __android_log_print(ANDROID_LOG_INFO, "GameRenderer",
                      "onSurfaceChanged: Width: %d, Height: %d", _width,
                      _height);
}

void GameRenderer::onSurfaceDestroyed() {
  if (_eglDisplay != EGL_NO_DISPLAY) {
    eglMakeCurrent(_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    if (_eglSurface != EGL_NO_SURFACE) {
      eglDestroySurface(_eglDisplay, _eglSurface);
      _eglSurface = EGL_NO_SURFACE;
    }
  }

  if (_nativeWindow) {
    ANativeWindow_release(_nativeWindow);
    _nativeWindow = nullptr;
  }

  _surface = nullptr;
  _width = 0;
  _height = 0;

  __android_log_print(ANDROID_LOG_INFO, "GameRenderer", "onSurfaceDestroyed");
}

void GameRenderer::render(const Screen &screen,
                          const std::map<std::string, Entity> &entities) {

  __android_log_print(ANDROID_LOG_DEBUG, "GameRenderer",
                      "render: called, entities=%zu", entities.size());

  if (!_grContext || _eglSurface == EGL_NO_SURFACE || _width == 0 ||
      _height == 0) {
    __android_log_print(ANDROID_LOG_WARN, "GameRenderer",
                        "render: early-return: guard failed");
    return;
  }

  if (!eglMakeCurrent(_eglDisplay, _eglSurface, _eglSurface, _eglContext)) {
    __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                        "render: eglMakeCurrent failed: 0x%x", eglGetError());
    return;
  }

  if (!_surface) {
    if (!createSkSurface()) {
      return;
    }
  }

  SkCanvas *canvas = _surface->getCanvas();

  canvas->clear(SK_ColorBLACK);

  float virtualWidth = static_cast<float>(screen.width);
  float virtualHeight = static_cast<float>(screen.height);

  if (virtualWidth <= 0.0f || virtualHeight <= 0.0f) {
    return;
  }

  float scaleX = static_cast<float>(_width) / virtualWidth;
  float scaleY = static_cast<float>(_height) / virtualHeight;

  float scale = std::min(scaleX, scaleY);

  float offsetX = (static_cast<float>(_width) - (virtualWidth * scale)) * 0.5f;
  float offsetY =
      (static_cast<float>(_height) - (virtualHeight * scale)) * 0.5f;

  canvas->save();
  canvas->translate(offsetX, offsetY);
  canvas->scale(scale, scale);

  SkRect gameBounds = SkRect::MakeWH(virtualWidth, virtualHeight);
  canvas->clipRect(gameBounds);

  if (screen.color.has_value()) {
    canvas->clear(ColorUtils::parseHexColor(screen.color.value()));
  }

  if (screen.asset.has_value() && screen.asset.value() != 0) {
    double assetId = screen.asset.value();

    auto svgIt = _svgCache.find(assetId);
    if (svgIt != _svgCache.end() && svgIt->second) {
      auto &svgDom = svgIt->second;

      SkSize intrinsicSize = svgDom->containerSize();
      if (intrinsicSize.isEmpty()) {
        svgDom->setContainerSize(SkSize::Make(virtualWidth, virtualHeight));
        intrinsicSize = SkSize::Make(virtualWidth, virtualHeight);
      }

      float scaleX = virtualWidth / intrinsicSize.width();
      float scaleY = virtualHeight / intrinsicSize.height();

      canvas->save();
      canvas->scale(scaleX, scaleY);
      svgDom->render(canvas);
      canvas->restore();
    } else {
      auto lottieIt = _lottieCache.find(assetId);
      if (lottieIt != _lottieCache.end() && lottieIt->second) {
        auto &lottie = lottieIt->second;
        lottie->seekFrameTime(screen.progress.value_or(0.0) *
                              lottie->duration());
        SkRect dstBounds = SkRect::MakeWH(virtualWidth, virtualHeight);
        lottie->render(canvas, &dstBounds);
      }
    }
  }

  for (const auto &[id, entity] : entities) {
    if (!isEntityVisible(entity, virtualWidth, virtualHeight)) {
      continue;
    }

    if (entity.color.has_value() ||
        (entity.asset.has_value() && entity.asset.value() != 0)) {
      float px = static_cast<float>(entity.px);
      float py = static_cast<float>(entity.py);

      if (entity.color.has_value()) {
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setStyle(SkPaint::kFill_Style);
        paint.setColor(ColorUtils::parseHexColor(entity.color.value()));

        std::visit(
            [&](const auto &shape) {
              using T = std::decay_t<decltype(shape)>;
              if constexpr (std::is_same_v<T, Rect>) {
                float width = static_cast<float>(shape.width);
                float height = static_cast<float>(shape.height);
                SkRect rect = SkRect::MakeXYWH(px - width / 2, py - height / 2,
                                               width, height);
                canvas->drawRect(rect, paint);
              } else {
                float radius = static_cast<float>(shape.radius);
                canvas->drawCircle(px, py, radius, paint);
              }
            },
            entity.shape);
      }

      if ((entity.asset.has_value() && entity.asset.value() != 0)) {
        double assetId = entity.asset.value();

        float width = 0.f, height = 0.f;
        std::visit(
            [&](const auto &shape) {
              using T = std::decay_t<decltype(shape)>;
              if constexpr (std::is_same_v<T, Rect>) {
                width = static_cast<float>(shape.width);
                height = static_cast<float>(shape.height);
              } else {
                float diameter = static_cast<float>(shape.radius) * 2.f;
                width = diameter;
                height = diameter;
              }
            },
            entity.shape);

        SkAutoCanvasRestore autoRestore(canvas, true);
        canvas->translate(px - width / 2, py - height / 2);

        auto svgIt = _svgCache.find(assetId);
        if (svgIt != _svgCache.end() && svgIt->second) {
          auto &svgDom = svgIt->second;

          SkSize intrinsicSize = svgDom->containerSize();
          if (intrinsicSize.isEmpty()) {
            svgDom->setContainerSize(SkSize::Make(width, height));
            intrinsicSize = SkSize::Make(width, height);
          }

          float scaleX = width / intrinsicSize.width();
          float scaleY = height / intrinsicSize.height();

          canvas->save();
          canvas->scale(scaleX, scaleY);
          svgDom->render(canvas);
          canvas->restore();
        } else {
          auto lottieIt = _lottieCache.find(assetId);
          if (lottieIt != _lottieCache.end() && lottieIt->second) {
            auto &lottie = lottieIt->second;
            lottie->seekFrameTime(entity.progress.value_or(0.0) *
                                  lottie->duration());
            SkRect dstBounds = SkRect::MakeWH(width, height);
            lottie->render(canvas, &dstBounds);
          }
        }
      }
    }
  }

  canvas->restore();

  _grContext->flushAndSubmit(_surface.get());
  eglSwapBuffers(_eglDisplay, _eglSurface);

  __android_log_print(ANDROID_LOG_DEBUG, "GameRenderer", "render: complete");
}

bool GameRenderer::isEntityVisible(const Entity &entity, float screenWidth,
                                   float screenHeight) {
  float width = 0.f;
  float height = 0.f;

  std::visit(
      [&](const auto &shape) {
        using T = std::decay_t<decltype(shape)>;

        if constexpr (std::is_same_v<T, Rect>) {
          width = static_cast<float>(shape.width);
          height = static_cast<float>(shape.height);
        } else {
          width = height = static_cast<float>(shape.radius) * 2.f;
        }
      },
      entity.shape);

  float left = static_cast<float>(entity.px) - width / 2.f;
  float right = static_cast<float>(entity.px) + width / 2.f;
  float top = static_cast<float>(entity.py) - height / 2.f;
  float bottom = static_cast<float>(entity.py) + height / 2.f;

  return right > 0.f && left < screenWidth && bottom > 0.f &&
         top < screenHeight;
}

bool GameRenderer::initializeEGL() {
  _eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (_eglDisplay == EGL_NO_DISPLAY) {
    __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                        "initializeEGL: eglGetDisplay failed");
    return false;
  }

  if (!eglInitialize(_eglDisplay, nullptr, nullptr)) {
    __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                        "initializeEGL: eglInitialize failed");
    return false;
  }

  const EGLint configAttribs[] = {EGL_SURFACE_TYPE,
                                  EGL_WINDOW_BIT,
                                  EGL_RENDERABLE_TYPE,
                                  EGL_OPENGL_ES3_BIT,
                                  EGL_RED_SIZE,
                                  8,
                                  EGL_GREEN_SIZE,
                                  8,
                                  EGL_BLUE_SIZE,
                                  8,
                                  EGL_ALPHA_SIZE,
                                  8,
                                  EGL_NONE};

  EGLint numConfigs;
  if (!eglChooseConfig(_eglDisplay, configAttribs, &_eglConfig, 1,
                       &numConfigs) ||
      numConfigs == 0) {
    __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                        "initializeEGL: eglChooseConfig failed");
    return false;
  }

  const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

  _eglContext =
      eglCreateContext(_eglDisplay, _eglConfig, EGL_NO_CONTEXT, contextAttribs);
  if (_eglContext == EGL_NO_CONTEXT) {
    __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                        "initializeEGL: eglCreateContext failed");
    return false;
  }

  __android_log_print(ANDROID_LOG_INFO, "GameRenderer",
                      "initializeEGL: EGL initialized successfully");
  return true;
};

bool GameRenderer::createEGLSurface() {
  if (!_nativeWindow) {
    __android_log_print(
        ANDROID_LOG_ERROR, "GameRenderer",
        "createEGLSurface: No native window for surface creation");
    return false;
  }

  _eglSurface =
      eglCreateWindowSurface(_eglDisplay, _eglConfig, _nativeWindow, nullptr);
  if (_eglSurface == EGL_NO_SURFACE) {
    __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                        "createEGLSurface: eglCreateWindowSurface failed");
    return false;
  }

  if (!eglMakeCurrent(_eglDisplay, _eglSurface, _eglSurface, _eglContext)) {
    __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                        "createEGLSurface: eglMakeCurrent failed: 0x%x",
                        eglGetError());
    return false;
  }

  if (!_grContext) {
    auto glInterface = GrGLMakeNativeInterface();
    _grContext = GrDirectContexts::MakeGL(glInterface);
    if (!_grContext) {
      __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                          "createEGLSurface: GrDirectContext::MakeGL failed");
      return false;
    }
  }

  eglMakeCurrent(_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

  __android_log_print(ANDROID_LOG_INFO, "GameRenderer",
                      "createEGLSurface: EGL surface created");
  return true;
}

bool GameRenderer::createSkSurface() {
  if (_width == 0 || _height == 0) {
    return false;
  }

  GrGLFramebufferInfo fbInfo;
  fbInfo.fFBOID = 0;
  fbInfo.fFormat = GL_RGBA8;

  auto backendRT =
      GrBackendRenderTargets::MakeGL(_width, _height, 0, 8, fbInfo);

  SkSurfaceProps surfaceProps;
  _surface = SkSurfaces::WrapBackendRenderTarget(
      _grContext.get(), backendRT, kBottomLeft_GrSurfaceOrigin,
      kRGBA_8888_SkColorType, nullptr, &surfaceProps);

  if (!_surface) {
    __android_log_print(ANDROID_LOG_ERROR, "GameRenderer",
                        "createSkSurface: Failed to create SkSurface");
    return false;
  }

  return true;
}
} // namespace margelo::nitro::rngine
