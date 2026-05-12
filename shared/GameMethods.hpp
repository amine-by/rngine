#pragma once

#include "HybridGameMethodsSpec.hpp"

namespace margelo::nitro::rngine {
class GameMethods : public HybridGameMethodsSpec {
public:
  GameMethods() : HybridObject(TAG) {}
  void setTickRate(double tickRate) override;
  void setScreen(const Screen &screen) override;
  void setEntities(const std::vector<Entity> &entities) override;
  void setSystems(const std::vector<System> &systems) override;
  void pause() override;
  void resume() override;
  void spawn(const std::vector<Entity> &entities) override;
  void despawn(const std::string &id) override;
  void update(const std::vector<EntityUpdate> &updates) override;
};
} // namespace margelo::nitro::rngine
