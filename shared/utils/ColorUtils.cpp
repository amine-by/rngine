#include "ColorUtils.hpp"
#include <react/renderer/css/CSSColor.h>
#include <react/renderer/css/CSSValueParser.h>

namespace margelo::nitro::rngine::ColorUtils {
uint32_t parseString(const std::string &s) {
  auto v = facebook::react::parseCSSProperty<facebook::react::CSSColor>(s);
  if (auto *c = std::get_if<facebook::react::CSSColor>(&v)) {
    return (uint32_t(c->a) << 24) | (uint32_t(c->r) << 16) |
           (uint32_t(c->g) << 8) | c->b; // 0xAARRGGBB
  }
  return 0x00000000;
}
} // namespace margelo::nitro::rngine::ColorUtils
