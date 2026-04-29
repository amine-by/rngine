#pragma once

#include <cstdint>

namespace margelo::nitro::rngine {
struct GameStats {
  double tickRate = 0.0;
  double deltaTime = 0.0;
  uint64_t totalTicks = 0;
};
} // namespace margelo::nitro::rngine
