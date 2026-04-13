#pragma once

#include <cstdint>

namespace rngine {
struct GameStats {
  double fps = 0.0;
  double deltaTime = 0.0;
  uint64_t totalFrames = 0;
};
} // namespace rngine
