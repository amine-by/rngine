#include "AssetUtils.hpp"
#include "FitTransform.hpp"
#include "GameRenderer.hpp"
#include <variant>

namespace margelo::nitro::rngine::AssetUtils {
static FitTransform computeFitTransform(float srcW, float srcH,
                                        float targetWidth, float targetHeight,
                                        ObjectFit objectFit) {
  FitTransform tf;
  if (srcW <= 0.0f || srcH <= 0.0f || targetWidth <= 0.0f ||
      targetHeight <= 0.0f) {
    return tf;
  }

  switch (objectFit) {
  case ObjectFit::FILL: {
    tf.scaleX = targetWidth / srcW;
    tf.scaleY = targetHeight / srcH;
    break;
  }

  case ObjectFit::CONTAIN: {
    float scale = std::min(targetWidth / srcW, targetHeight / srcH);
    tf.scaleX = scale;
    tf.scaleY = scale;

    float drawW = srcW * scale;
    float drawH = srcH * scale;
    tf.translateX = (targetWidth - drawW) * 0.5f;
    tf.translateY = (targetHeight - drawH) * 0.5f;
    break;
  }

  case ObjectFit::COVER: {
    float scale = std::max(targetWidth / srcW, targetHeight / srcH);
    tf.scaleX = scale;
    tf.scaleY = scale;

    float drawW = srcW * scale;
    float drawH = srcH * scale;
    tf.translateX = (targetWidth - drawW) * 0.5f;
    tf.translateY = (targetHeight - drawH) * 0.5f;
    break;
  }

  case ObjectFit::NONE: {
    tf.scaleX = 1.0f;
    tf.scaleY = 1.0f;
    tf.translateX = (targetWidth - srcW) * 0.5f;
    tf.translateY = (targetHeight - srcH) * 0.5f;
    break;
  }
  }

  return tf;
}

static float computeCenteredTileStart(float targetSize, float tileSize) {
  if (tileSize <= 0.0f)
    return 0.0f;
  float center = targetSize * 0.5f;
  float offset = std::fmod(center, tileSize);
  return offset - tileSize;
}

static void drawSVG(SkCanvas *canvas, const sk_sp<SkSVGDOM> &svg,
                    float targetWidth, float targetHeight, ObjectFit objectFit,
                    bool clip, Repeat repeat) {
  SkSize intrinsicSize = svg->containerSize();
  if (intrinsicSize.isEmpty()) {
    svg->setContainerSize(SkSize::Make(targetWidth, targetHeight));
    intrinsicSize = SkSize::Make(targetWidth, targetHeight);
  }

  FitTransform tf =
      computeFitTransform(intrinsicSize.width(), intrinsicSize.height(),
                          targetWidth, targetHeight, objectFit);

  float tileW = intrinsicSize.width() * tf.scaleX;
  float tileH = intrinsicSize.height() * tf.scaleY;
  if (tileW <= 0.0f || tileH <= 0.0f)
    return;

  SkAutoCanvasRestore restore(canvas, true);

  bool isRepeating = (repeat != Repeat::NO_REPEAT);
  bool shouldClip = clip && (objectFit == ObjectFit::COVER ||
                             objectFit == ObjectFit::NONE || isRepeating);

  if (shouldClip) {
    canvas->clipRect(SkRect::MakeWH(targetWidth, targetHeight));
  }

  if (!isRepeating) {
    canvas->translate(tf.translateX, tf.translateY);
    canvas->scale(tf.scaleX, tf.scaleY);
    svg->render(canvas);
    return;
  }

  float startX = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_X)
                     ? computeCenteredTileStart(targetWidth, tileW)
                     : tf.translateX;

  float startY = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_Y)
                     ? computeCenteredTileStart(targetHeight, tileH)
                     : tf.translateY;

  float endX = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_X)
                   ? targetWidth
                   : (startX + tileW);

  float endY = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_Y)
                   ? targetHeight
                   : (startY + tileH);

  for (float y = startY; y < endY; y += tileH) {
    for (float x = startX; x < endX; x += tileW) {
      SkAutoCanvasRestore tileRestore(canvas, true);
      canvas->translate(x, y);
      canvas->scale(tf.scaleX, tf.scaleY);
      svg->render(canvas);
    }
  }
}

static void drawRaster(SkCanvas *canvas, const sk_sp<SkImage> &raster,
                       float targetWidth, float targetHeight,
                       ObjectFit objectFit, bool clip, Repeat repeat) {
  float intrinsicW = static_cast<float>(raster->width());
  float intrinsicH = static_cast<float>(raster->height());
  if (intrinsicW <= 0.0f || intrinsicH <= 0.0f)
    return;

  FitTransform tf = computeFitTransform(intrinsicW, intrinsicH, targetWidth,
                                        targetHeight, objectFit);

  SkAutoCanvasRestore restore(canvas, true);

  bool isRepeating = (repeat != Repeat::NO_REPEAT);
  bool shouldClip = clip && (objectFit == ObjectFit::COVER ||
                             objectFit == ObjectFit::NONE || isRepeating);

  if (shouldClip) {
    canvas->clipRect(SkRect::MakeWH(targetWidth, targetHeight));
  }

  SkSamplingOptions sampling(SkFilterMode::kLinear, SkMipmapMode::kLinear);

  if (!isRepeating) {
    canvas->translate(tf.translateX, tf.translateY);
    canvas->scale(tf.scaleX, tf.scaleY);
    canvas->drawImage(raster, 0.0f, 0.0f, sampling);
    return;
  }

  SkTileMode tileX = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_X)
                         ? SkTileMode::kRepeat
                         : SkTileMode::kClamp;
  SkTileMode tileY = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_Y)
                         ? SkTileMode::kRepeat
                         : SkTileMode::kClamp;

  float tileW = intrinsicW * tf.scaleX;
  float tileH = intrinsicH * tf.scaleY;

  SkMatrix localMatrix = SkMatrix::Scale(tf.scaleX, tf.scaleY);

  float offsetX = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_X)
                      ? computeCenteredTileStart(targetWidth, tileW)
                      : tf.translateX;
  float offsetY = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_Y)
                      ? computeCenteredTileStart(targetHeight, tileH)
                      : tf.translateY;

  localMatrix.postTranslate(offsetX, offsetY);

  auto shader = raster->makeShader(tileX, tileY, sampling, localMatrix);

  SkPaint paint;
  paint.setShader(shader);
  canvas->drawRect(SkRect::MakeWH(targetWidth, targetHeight), paint);
}

static void drawLottie(SkCanvas *canvas,
                       const sk_sp<skottie::Animation> &lottie,
                       float targetWidth, float targetHeight,
                       ObjectFit objectFit, bool clip, Repeat repeat,
                       double progress) {
  lottie->seek(progress);

  SkSize intrinsicSize = lottie->size();
  float intrinsicWidth = intrinsicSize.width();
  float intrinsicHeight = intrinsicSize.height();

  if (intrinsicWidth <= 0.0f || intrinsicHeight <= 0.0f)
    return;

  FitTransform tf = computeFitTransform(intrinsicWidth, intrinsicHeight,
                                        targetWidth, targetHeight, objectFit);

  float tileW = intrinsicWidth * tf.scaleX;
  float tileH = intrinsicHeight * tf.scaleY;
  if (tileW <= 0.0f || tileH <= 0.0f)
    return;

  SkAutoCanvasRestore restore(canvas, true);

  bool isRepeating = (repeat != Repeat::NO_REPEAT);
  bool shouldClip = clip && (objectFit == ObjectFit::COVER ||
                             objectFit == ObjectFit::NONE || isRepeating);

  if (shouldClip) {
    canvas->clipRect(SkRect::MakeWH(targetWidth, targetHeight));
  }

  SkRect dstBounds = SkRect::MakeWH(intrinsicWidth, intrinsicHeight);

  if (!isRepeating) {
    canvas->translate(tf.translateX, tf.translateY);
    canvas->scale(tf.scaleX, tf.scaleY);
    lottie->render(canvas, &dstBounds);
    return;
  }

  float startX = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_X)
                     ? computeCenteredTileStart(targetWidth, tileW)
                     : tf.translateX;
  float startY = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_Y)
                     ? computeCenteredTileStart(targetHeight, tileH)
                     : tf.translateY;

  float endX = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_X)
                   ? targetWidth
                   : (startX + tileW);
  float endY = (repeat == Repeat::REPEAT || repeat == Repeat::REPEAT_Y)
                   ? targetHeight
                   : (startY + tileH);

  for (float y = startY; y < endY; y += tileH) {
    for (float x = startX; x < endX; x += tileW) {
      SkAutoCanvasRestore tileRestore(canvas, true);
      canvas->translate(x, y);
      canvas->scale(tf.scaleX, tf.scaleY);
      lottie->render(canvas, &dstBounds);
    }
  }
}

void drawAsset(SkCanvas *canvas, double asset, float targetWidth,
               float targetHeight, bool flipH, bool flipV, ObjectFit objectFit,
               bool clip, Repeat repeat, double progress) {
  if (asset == 0) {
    return;
  }

  SkAutoCanvasRestore restoreGuard(canvas, true);

  if (flipH || flipV) {
    canvas->translate(targetWidth * 0.5f, targetHeight * 0.5f);
    canvas->scale(flipH ? -1.0f : 1.0f, flipV ? -1.0f : 1.0f);
    canvas->translate(-targetWidth * 0.5f, -targetHeight * 0.5f);
  }

  auto &gameRenderer = GameRenderer::getInstance();

  if (asset > 0) {
    auto &imageCache = gameRenderer.getImageCacheInternal();
    auto imageIt = imageCache.find(asset);
    if (imageIt != imageCache.end()) {
      std::visit(
          [&](const auto &image) {
            using T = std::decay_t<decltype(image)>;
            if constexpr (std::is_same_v<T, sk_sp<SkSVGDOM>>) {
              drawSVG(canvas, image, targetWidth, targetHeight, objectFit, clip,
                      repeat);
            } else {
              drawRaster(canvas, image, targetWidth, targetHeight, objectFit,
                         clip, repeat);
            }
          },
          imageIt->second);
      return;
    }
  }

  auto &lottieCache = gameRenderer.getLottieCacheInternal();
  auto lottieIt = lottieCache.find(asset);
  if (lottieIt != lottieCache.end() && lottieIt->second) {
    drawLottie(canvas, lottieIt->second, targetWidth, targetHeight, objectFit,
               clip, repeat, progress);
  }
}

void updateProgress(std::optional<double> &progress,
                    const std::optional<double> &speed,
                    const std::optional<bool> &loop, const double asset,
                    const double deltaTime) {
  if (asset < 0) {
    auto &lottieCache = GameRenderer::getInstance().getLottieCacheInternal();
    auto it = lottieCache.find(asset);
    if (it != lottieCache.end() && it->second) {
      if (speed.has_value() && speed.value() > 0) {
        double newProgress =
            progress.value_or(0.0) +
            (deltaTime * speed.value()) / it->second->duration();

        if (loop.value_or(false)) {
          progress = std::fmod(newProgress, 1.0);
        } else {
          progress = std::min(newProgress, 1.0);
        }
      }
    }
  } else if (progress.has_value()) {
    progress.reset();
  }
}
} // namespace margelo::nitro::rngine::AssetUtils
