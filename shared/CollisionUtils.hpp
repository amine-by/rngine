#pragma once

#include "Circle.hpp"
#include "Entity.hpp"
#include "Rect.hpp"
#include <optional>

namespace margelo::nitro::rngine::CollisionUtils {
std::optional<double> overlap(const Rect &a, double ax, double ay,
                              const Rect &b, double bx, double by);
std::optional<double> overlap(const Circle &a, double ax, double ay,
                              const Circle &b, double bx, double by);
std::optional<double> overlap(const Rect &a, double ax, double ay,
                              const Circle &b, double bx, double by);
std::optional<double> overlap(const Circle &a, double ax, double ay,
                              const Rect &b, double bx, double by);
std::optional<double> shapeOverlap(const Entity &entityA,
                                   const Entity &entityB);
} // namespace margelo::nitro::rngine::CollisionUtils
