#include "AssetUtils.hpp"
#include "GameRenderer.hpp"
#include <variant>

namespace margelo::nitro::rngine::AssetUtils {
void drawSVG(SkCanvas *canvas, const sk_sp<SkSVGDOM> &svg, float width,
             float height) {
  SkSize intrinsicSize = svg->containerSize();
  if (intrinsicSize.isEmpty()) {
    svg->setContainerSize(SkSize::Make(width, height));
    intrinsicSize = SkSize::Make(width, height);
  }

  SkAutoCanvasRestore restore(canvas, true);
  canvas->scale(width / intrinsicSize.width(), height / intrinsicSize.height());
  svg->render(canvas);
}

void drawRaster(SkCanvas *canvas, const sk_sp<SkImage> &raster, float width,
                float height) {
  float intrinsicW = static_cast<float>(raster->width());
  float intrinsicH = static_cast<float>(raster->height());
  if (intrinsicW <= 0.0f || intrinsicH <= 0.0f)
    return;

  SkAutoCanvasRestore restore(canvas, true);
  canvas->scale(width / intrinsicW, height / intrinsicH);
  SkSamplingOptions sampling(SkFilterMode::kLinear, SkMipmapMode::kLinear);
  canvas->drawImage(raster, 0.0f, 0.0f, sampling);
}

void drawAsset(SkCanvas *canvas, double asset, float targetWidth,
               float targetHeight, bool flipH, bool flipV, double progress) {
  SkAutoCanvasRestore restoreGuard(canvas, true);

  if (flipH || flipV) {
    canvas->translate(targetWidth * 0.5f, targetHeight * 0.5f);
    canvas->scale(flipH ? -1.0f : 1.0f, flipV ? -1.0f : 1.0f);
    canvas->translate(-targetWidth * 0.5f, -targetHeight * 0.5f);
  }

  if (asset == 0) {
    return;
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
              drawSVG(canvas, image, targetWidth, targetHeight);
            } else {
              drawRaster(canvas, image, targetWidth, targetHeight);
            }
          },
          imageIt->second);
      return;
    }
  }

  auto &lottieCache = gameRenderer.getLottieCacheInternal();
  auto lottieIt = lottieCache.find(asset);
  if (lottieIt != lottieCache.end() && lottieIt->second) {
    lottieIt->second->seek(progress);
    SkRect dstBounds = SkRect::MakeWH(targetWidth, targetHeight);
    lottieIt->second->render(canvas, &dstBounds);
  }
}

void updateProgress(std::optional<double> &progress,
                    std::optional<double> &speed, double asset,
                    double deltaTime) {
  if (asset < 0) {
    auto &lottieCache = GameRenderer::getInstance().getLottieCacheInternal();
    auto it = lottieCache.find(asset);
    if (it != lottieCache.end() && it->second) {
      if (speed.has_value() && speed.value() > 0) {
        progress = fmod(progress.value_or(0.0) + (deltaTime * speed.value()) /
                                                     it->second->duration(),
                        1.0);
      }
    }
  } else if (progress.has_value()) {
    progress.reset();
  }
}
} // namespace margelo::nitro::rngine::AssetUtils
