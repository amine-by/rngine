#pragma once

#include "HybridGameMethodsSpec.hpp"

namespace margelo::nitro::rngine {
class GameMethods : public HybridGameMethodsSpec {
public:
  GameMethods() : HybridObject(TAG) {}
  void setTickRate(double tickRate) override;
  void setWorld(const World &world) override;
  void setEntities(const std::vector<Entity> &entities) override;
  void setSystems(const std::vector<System> &systems) override;
  void pause() override;
  void resume() override;
  void spawn(const std::vector<Entity> &entities) override;
  void despawn(const std::string &id) override;
  void setP(const std::string &id, double px, double py) override;
  void setV(const std::string &id, double vx, double vy) override;
};
} // namespace margelo::nitro::rngine
