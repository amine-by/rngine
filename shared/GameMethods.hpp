#pragma once

#include "HybridGameMethodsSpec.hpp"

namespace margelo::nitro::rngine {
class GameMethods : public HybridGameMethodsSpec {
public:
  GameMethods() : HybridObject(TAG) {}
  void setV(const std::string &id, double xv, double yv) override;
};
} // namespace margelo::nitro::rngine
