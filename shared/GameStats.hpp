#pragma once

#include <cstdint>

namespace margelo::nitro::rngine {
struct GameStats {
  double fps = 0.0;
  double deltaTime = 0.0;
  uint64_t totalFrames = 0;
};
} // namespace margelo::nitro::rngine
