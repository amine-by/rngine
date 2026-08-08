#pragma once
#include <string>

namespace margelo::nitro::rngine {

inline uint32_t parseHexColor(const std::optional<std::string> &color) {
  if (!color.has_value())
    return 0x00000000;
  const std::string &c = color.value();
  if (!c.empty() && c[0] == '#') {
    std::string hex = c.substr(1);

    if (hex.size() == 3 || hex.size() == 4) {
      std::string expanded;
      expanded.reserve(hex.size() * 2);
      for (char c : hex) {
        expanded += c;
        expanded += c;
      }
      hex = std::move(expanded);
    }

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
  }

  return 0x00000000;
}
} // namespace margelo::nitro::rngine
