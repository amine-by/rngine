#pragma once
#include "include/core/SkCanvas.h"
#include "modules/svg/include/SkSVGDOM.h"

namespace margelo::nitro::rngine::AssetUtils {
void drawAsset(SkCanvas *canvas, double asset, float targetWidth,
               float targetHeight, bool flipH, bool flipV, double progress);
void updateProgress(std::optional<double> &progress,
                    std::optional<double> &speed, double asset,
                    double deltaTime);
} // namespace margelo::nitro::rngine::AssetUtils
