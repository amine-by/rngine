#pragma once
#include <stdexcept>
#include <string>

namespace margelo::nitro::rngine {

inline uint32_t parseHexColor(const std::string &color) {
  if (color.empty() || color[0] != '#') {
    throw std::invalid_argument("Only hex colors supported");
  }
  std::string hex = color.substr(1);
  if (hex.size() == 6) {
    return 0xFF000000 | std::stoul(hex, nullptr, 16);
  } else if (hex.size() == 8) {
    uint32_t rgba = std::stoul(hex, nullptr, 16);
    uint32_t r = (rgba >> 24) & 0xFF;
    uint32_t g = (rgba >> 16) & 0xFF;
    uint32_t b = (rgba >> 8) & 0xFF;
    uint32_t a = rgba & 0xFF;
    return (a << 24) | (r << 16) | (g << 8) | b;
  }
  throw std::invalid_argument("Invalid hex color: " + color);
}

} // namespace margelo::nitro::rngine
