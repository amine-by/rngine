#pragma once

#include "HybridGameMethodsSpec.hpp"
#include "World.hpp"

namespace margelo::nitro::rngine {
class GameMethods : public HybridGameMethodsSpec {
public:
  GameMethods() : HybridObject(TAG) {}

  void setWorld(const World &world) override;

  void setScreen(const Screen &screen) override;

  void setEntities(const std::vector<Entity> &entities) override;

  void setSystems(const std::vector<System> &systems) override;

  void pause() override;

  void resume() override;

  void spawn(const std::vector<Entity> &entities) override;

  void despawn(const std::string &id) override;

  void update(const std::vector<EntityUpdate> &updates) override;

  bool isAssetLoaded(double id) override;

  std::shared_ptr<Promise<bool>>
  loadLottie(double id, const std::string &jsonStr) override;

  std::shared_ptr<Promise<bool>> loadImage(double id,
                                           const std::string &svgUri) override;
};
} // namespace margelo::nitro::rngine
