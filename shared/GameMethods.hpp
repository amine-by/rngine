#pragma once

#include "EntityInitializer.hpp"
#include "HybridGameMethodsSpec.hpp"
#include "ScreenInitializer.hpp"
#include "WorldInitializer.hpp"

namespace margelo::nitro::rngine {
class GameMethods : public HybridGameMethodsSpec {
public:
  GameMethods() : HybridObject(TAG) {}

  void setWorld(const WorldInitializer &WorldInitializer) override;

  void setScreen(const ScreenInitializer &screenInitializer) override;

  void setEntities(
      const std::vector<EntityInitializer> &entitiesInitializer) override;

  void setSystems(const std::vector<System> &systems) override;

  void pause() override;

  void resume() override;

  void
  spawn(const std::vector<EntityInitializer> &entitiesInitializer) override;

  void despawn(const std::string &id) override;

  void updateEntities(const std::vector<EntityUpdate> &entityUpdates) override;

  void updateScreen(const ScreenUpdate &screenUpdate) override;

  bool isAssetLoaded(double id) override;

  std::shared_ptr<Promise<bool>>
  loadLottie(double id, const std::string &jsonStr) override;

  std::shared_ptr<Promise<bool>> loadImage(double id,
                                           const std::string &svgUri) override;
};
} // namespace margelo::nitro::rngine
