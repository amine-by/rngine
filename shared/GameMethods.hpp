#pragma once

#include "HybridGameMethodsSpec.hpp"

namespace margelo::nitro::rngine {
class GameMethods : public HybridGameMethodsSpec {
public:
  GameMethods() : HybridObject(TAG) {}
  void setP(const std::string &id, double px, double py) override;
  void setV(const std::string &id, double vx, double vy) override;
};
} // namespace margelo::nitro::rngine
