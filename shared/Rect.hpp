#pragma once

namespace margelo::nitro::rngine {
static constexpr int RECT_SIZE = sizeof(float) * 4 + sizeof(uint32_t) * 2;

struct Rect {
  float left;
  float right;
  float top;
  float bottom;
  uint32_t color;
  uint32_t asset;
};
} // namespace margelo::nitro::rngine
