#pragma once

namespace margelo::nitro::rngine {
static constexpr int RECT_SIZE = sizeof(double) * 5 + sizeof(uint32_t);

struct Rect {
  double left;
  double right;
  double top;
  double bottom;
  uint32_t color;
  double asset;
};
} // namespace margelo::nitro::rngine
