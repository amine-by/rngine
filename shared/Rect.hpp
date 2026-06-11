#pragma once

namespace margelo::nitro::rngine {
static constexpr int RECT_SIZE =
    sizeof(float) * 5 + sizeof(uint32_t) + sizeof(int32_t);

struct Rect {
  float left;
  float right;
  float top;
  float bottom;
  float progress;
  uint32_t color;
  int32_t asset;
};
} // namespace margelo::nitro::rngine
