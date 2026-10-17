#include "CollisionUtils.hpp"
#include "Collision.hpp"
#include "Overlap.hpp"
#include <optional>

namespace margelo::nitro::rngine::CollisionUtils {
static std::optional<Overlap> overlap(const Rect &a, double ax, double ay,
                                      double aa, const Rect &b, double bx,
                                      double by, double ba) {
  const double dx = bx - ax, dy = by - ay;
  const double ahw = a.width / 2.0, ahh = a.height / 2.0;
  const double bhw = b.width / 2.0, bhh = b.height / 2.0;

  const double ac = std::cos(aa), as = std::sin(aa);
  const double bc = std::cos(ba), bs = std::sin(ba);

  const double aux = ac, auy = as, avx = -as, avy = ac;
  const double bux = bc, buy = bs, bvx = -bs, bvy = bc;

  const double axes[4][2] = {{aux, auy}, {avx, avy}, {bux, buy}, {bvx, bvy}};

  double minDepth = std::numeric_limits<double>::infinity();
  double bestNx = 0.0, bestNy = 0.0;

  for (const auto &axis : axes) {
    const double nx = axis[0], ny = axis[1];

    const double ra = ahw * std::abs(nx * aux + ny * auy) +
                      ahh * std::abs(nx * avx + ny * avy);
    const double rb = bhw * std::abs(nx * bux + ny * buy) +
                      bhh * std::abs(nx * bvx + ny * bvy);

    const double dist = dx * nx + dy * ny;
    const double depth = ra + rb - std::abs(dist);

    if (depth <= 0)
      return std::nullopt;

    if (depth < minDepth) {
      minDepth = depth;
      const double sign = (dist >= 0) ? 1.0 : -1.0;
      bestNx = nx * sign;
      bestNy = ny * sign;
    }
  }

  return Overlap{minDepth, bestNx, bestNy};
}

static std::optional<Overlap> overlap(const Circle &a, double ax, double ay,
                                      double aa, const Circle &b, double bx,
                                      double by, double ba) {
  double dx = bx - ax, dy = by - ay;
  double dist = std::sqrt(dx * dx + dy * dy);
  double depth = (a.radius + b.radius) - dist;

  if (depth <= 0)
    return std::nullopt;

  if (dist == 0.0)
    return Overlap{depth, 0.0, -1.0};

  return Overlap{depth, dx / dist, dy / dist};
}

static std::optional<Overlap> overlap(const Rect &a, double ax, double ay,
                                      double aa, const Circle &b, double bx,
                                      double by, double ba) {
  const double hw = a.width / 2.0, hh = a.height / 2.0;
  const double c = std::cos(aa), s = std::sin(aa);

  const double wx = bx - ax, wy = by - ay;
  const double lx = wx * c + wy * s;
  const double ly = -wx * s + wy * c;

  const double closestX = std::clamp(lx, -hw, hw);
  const double closestY = std::clamp(ly, -hh, hh);
  const double dx = lx - closestX, dy = ly - closestY;
  const double distSq = dx * dx + dy * dy;

  if (distSq >= b.radius * b.radius)
    return std::nullopt;

  double nlx, nly, depth;

  if (distSq == 0.0) {
    const double px = hw - std::abs(lx);
    const double py = hh - std::abs(ly);
    if (px < py) {
      nlx = (lx >= 0) ? 1.0 : -1.0;
      nly = 0.0;
      depth = px + b.radius;
    } else {
      nlx = 0.0;
      nly = (ly >= 0) ? 1.0 : -1.0;
      depth = py + b.radius;
    }
  } else {
    const double dist = std::sqrt(distSq);
    nlx = dx / dist;
    nly = dy / dist;
    depth = b.radius - dist;
  }

  const double nx = nlx * c - nly * s;
  const double ny = nlx * s + nly * c;
  return Overlap{depth, nx, ny};
}

static std::optional<Overlap> overlap(const Circle &a, double ax, double ay,
                                      double aa, const Rect &b, double bx,
                                      double by, double ba) {
  auto result = overlap(b, bx, by, ba, a, ax, ay, aa);

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
              return overlap(shapeA, entityA.px, entityA.py,
                             entityA.angle.value_or(0.0), shapeB, entityB.px,
                             entityB.py, entityB.angle.value_or(0.0));
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
