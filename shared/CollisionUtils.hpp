#pragma once

#include "Circle.hpp"
#include "Collision.hpp"
#include "Entity.hpp"
#include "Rect.hpp"
#include <optional>

namespace margelo::nitro::rngine::CollisionUtils {
struct Overlap {
  double depth;
  double nx;
  double ny;
};

std::optional<Overlap> overlap(const Rect &a, double ax, double ay,
                               const Rect &b, double bx, double by);
std::optional<Overlap> overlap(const Circle &a, double ax, double ay,
                               const Circle &b, double bx, double by);
std::optional<Overlap> overlap(const Rect &a, double ax, double ay,
                               const Circle &b, double bx, double by);
std::optional<Overlap> overlap(const Circle &a, double ax, double ay,
                               const Rect &b, double bx, double by);
std::optional<Collision> shapeOverlap(const Entity &entityA,
                                      const Entity &entityB);
} // namespace margelo::nitro::rngine::CollisionUtils
