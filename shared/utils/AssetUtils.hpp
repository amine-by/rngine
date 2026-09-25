#pragma once
#include "include/core/SkCanvas.h"
#include "modules/svg/include/SkSVGDOM.h"

namespace margelo::nitro::rngine::AssetUtils {
void drawAsset(SkCanvas *canvas, double asset, float targetWidth,
               float targetHeight, bool flipH, bool flipV, double progress);
void updateProgress(std::optional<double> &progress,
                    const std::optional<double> &speed,
                    const std::optional<bool> &loop, const double asset,
                    const double deltaTime);
} // namespace margelo::nitro::rngine::AssetUtils
