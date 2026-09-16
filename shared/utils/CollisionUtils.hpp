#pragma once

#include "Collision.hpp"
#include "Entity.hpp"
#include <optional>

namespace margelo::nitro::rngine::CollisionUtils {
std::optional<Collision> shapeOverlap(const Entity &entityA,
                                      const Entity &entityB);
} // namespace margelo::nitro::rngine::CollisionUtils
