#include "CollisionUtils.hpp"
#include "Collision.hpp"
#include <optional>

namespace margelo::nitro::rngine::CollisionUtils {
std::optional<Overlap> overlap(const Rect &a, double ax, double ay,
                               const Rect &b, double bx, double by) {

  double aLeft = ax - a.width / 2.0, aRight = ax + a.width / 2.0;
  double aTop = ay - a.height / 2.0, aBottom = ay + a.height / 2.0;
  double bLeft = bx - b.width / 2.0, bRight = bx + b.width / 2.0;
  double bTop = by - b.height / 2.0, bBottom = by + b.height / 2.0;

  double overlapX = std::min(aRight, bRight) - std::max(aLeft, bLeft);
  double overlapY = std::min(aBottom, bBottom) - std::max(aTop, bTop);

  if (overlapX <= 0 || overlapY <= 0)
    return std::nullopt;

  if (overlapX < overlapY) {
    double nx = (bx >= ax) ? 1.0 : -1.0;
    return Overlap{overlapX, nx, 0.0};
  } else {
    double ny = (by >= ay) ? 1.0 : -1.0;
    return Overlap{overlapY, 0.0, ny};
  }
}

std::optional<Overlap> overlap(const Circle &a, double ax, double ay,
                               const Circle &b, double bx, double by) {
  double dx = bx - ax, dy = by - ay;
  double dist = std::sqrt(dx * dx + dy * dy);
  double depth = (a.radius + b.radius) - dist;

  if (depth <= 0)
    return std::nullopt;

  if (dist == 0.0)
    return Overlap{depth, 0.0, -1.0};

  return Overlap{depth, dx / dist, dy / dist};
}

std::optional<Overlap> overlap(const Rect &a, double ax, double ay,
                               const Circle &b, double bx, double by) {
  double left = ax - a.width / 2.0, right = ax + a.width / 2.0;
  double top = ay - a.height / 2.0, bottom = ay + a.height / 2.0;

  double closestX = std::clamp(bx, left, right);
  double closestY = std::clamp(by, top, bottom);

  double dx = bx - closestX, dy = by - closestY;
  double distSq = dx * dx + dy * dy;

  if (distSq >= b.radius * b.radius)
    return std::nullopt;

  if (distSq == 0.0) {
    double cdx = bx - ax, cdy = by - ay;
    double cdist = std::sqrt(cdx * cdx + cdy * cdy);
    if (cdist == 0.0)
      return Overlap{b.radius, 0.0, -1.0};
    return Overlap{b.radius, cdx / cdist, cdy / cdist};
  }

  double dist = std::sqrt(distSq);
  return Overlap{b.radius - dist, dx / dist, dy / dist};
}

std::optional<Overlap> overlap(const Circle &a, double ax, double ay,
                               const Rect &b, double bx, double by) {
  auto result = overlap(b, bx, by, a, ax, ay);

  if (!result.has_value()) {
    return std::nullopt;
  }

  result.value().nx = -result.value().nx;
  result.value().ny = -result.value().ny;

  return result;
}

std::optional<Collision> shapeOverlap(const Entity &entityA,
                                      const Entity &entityB) {
  auto result = std::visit(
      [&](const auto &shapeA) -> std::optional<Overlap> {
        return std::visit(
            [&](const auto &shapeB) -> std::optional<Overlap> {
              return overlap(shapeA, entityA.px, entityA.py, shapeB, entityB.px,
                             entityB.py);
            },
            entityB.shape);
      },
      entityA.shape);
  if (!result.has_value()) {
    return nullopt;
  }

  return Collision{entityA.id, entityB.id, result->depth, result->nx,
                   result->ny};
}
} // namespace margelo::nitro::rngine::CollisionUtils
