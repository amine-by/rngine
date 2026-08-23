#include "CollisionUtils.hpp"

namespace margelo::nitro::rngine::CollisionUtils {
std::optional<double> overlap(const Rect &a, double ax, double ay,
                              const Rect &b, double bx, double by) {

  double aLeft = ax - a.width / 2.0, aRight = ax + a.width / 2.0;
  double aTop = ay - a.height / 2.0, aBottom = ay + a.height / 2.0;
  double bLeft = bx - b.width / 2.0, bRight = bx + b.width / 2.0;
  double bTop = by - b.height / 2.0, bBottom = by + b.height / 2.0;

  double overlapX = std::min(aRight, bRight) - std::max(aLeft, bLeft);
  double overlapY = std::min(aBottom, bBottom) - std::max(aTop, bTop);

  if (overlapX > 0 && overlapY > 0)
    return std::min(overlapX, overlapY);
  return std::nullopt;
}

std::optional<double> overlap(const Circle &a, double ax, double ay,
                              const Circle &b, double bx, double by) {
  double dx = bx - ax, dy = by - ay;
  double dist = std::sqrt(dx * dx + dy * dy);
  double depth = (a.radius + b.radius) - dist;

  if (depth > 0)
    return depth;
  return std::nullopt;
}

std::optional<double> overlap(const Rect &a, double ax, double ay,
                              const Circle &b, double bx, double by) {
  double left = ax - a.width / 2.0, right = ax + a.width / 2.0;
  double top = ay - a.height / 2.0, bottom = ay + a.height / 2.0;

  double closestX = std::clamp(bx, left, right);
  double closestY = std::clamp(by, top, bottom);

  double dx = bx - closestX, dy = by - closestY;
  double distSq = dx * dx + dy * dy;

  if (distSq >= b.radius * b.radius)
    return std::nullopt;

  double dist = std::sqrt(distSq);
  return b.radius - dist;
}

std::optional<double> overlap(const Circle &a, double ax, double ay,
                              const Rect &b, double bx, double by) {
  return overlap(b, bx, by, a, ax, ay);
}

std::optional<double> shapeOverlap(const Entity &entityA,
                                   const Entity &entityB) {
  return std::visit(
      [&](const auto &shapeA) -> std::optional<double> {
        return std::visit(
            [&](const auto &shapeB) -> std::optional<double> {
              return overlap(shapeA, entityA.px, entityA.py, shapeB, entityB.px,
                             entityB.py);
            },
            entityB.shape);
      },
      entityA.shape);
}
} // namespace margelo::nitro::rngine::CollisionUtils
