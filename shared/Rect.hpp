#pragma once

namespace margelo::nitro::rngine {
static constexpr int RECT_SIZE = sizeof(double) * 4;

struct Rect {
  double left;
  double right;
  double top;
  double bottom;
};
} // namespace margelo::nitro::rngine
