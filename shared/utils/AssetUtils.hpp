#pragma once
#include "ObjectFit.hpp"
#include "Repeat.hpp"
#include "include/core/SkCanvas.h"
#include "modules/svg/include/SkSVGDOM.h"

namespace margelo::nitro::rngine::AssetUtils {
void drawAsset(SkCanvas *canvas, double asset, float targetWidth,
               float targetHeight, bool flipH, bool flipV, ObjectFit objectFit,
               bool clip, Repeat repeat, double progress);
void updateProgress(std::optional<double> &progress,
                    const std::optional<double> &speed,
                    const std::optional<bool> &loop, const double asset,
                    const double deltaTime);
} // namespace margelo::nitro::rngine::AssetUtils
