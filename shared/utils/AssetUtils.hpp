#pragma once
#include "ObjectFit.hpp"
#include "Repeat.hpp"
#include "include/core/SkCanvas.h"
#include "modules/svg/include/SkSVGDOM.h"

namespace margelo::nitro::rngine::AssetUtils {
void drawAsset(SkCanvas *canvas, double asset, float targetWidth,
               float targetHeight, bool flipH, bool flipV, double angle,
               ObjectFit objectFit, bool clip, Repeat repeat, double progress);
void updateProgress(double &progress, const double speed, const bool loop,
                    const double asset, const double deltaTime);
} // namespace margelo::nitro::rngine::AssetUtils
