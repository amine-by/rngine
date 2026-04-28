#pragma once

namespace margelo::nitro::rngine {
static constexpr int RECT_SIZE = sizeof(double) * 4 + sizeof(uint32_t);

struct Rect {
  double left;
  double right;
  double top;
  double bottom;
  uint32_t color;
};
} // namespace margelo::nitro::rngine
