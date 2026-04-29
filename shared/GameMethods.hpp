#pragma once

#include "HybridGameMethodsSpec.hpp"

namespace margelo::nitro::rngine {
class GameMethods : public HybridGameMethodsSpec {
public:
  GameMethods() : HybridObject(TAG) {}
  void initialize(bool isPaused, double tickRate, const World &world,
                  const std::vector<Entity> &entities,
                  const std::vector<System> &systems) override;
  void pause() override;
  void resume() override;
  void spawn(const Entity &entity) override;
  void despawn(const std::string &id) override;
  void setP(const std::string &id, double px, double py) override;
  void setV(const std::string &id, double vx, double vy) override;
};
} // namespace margelo::nitro::rngine
