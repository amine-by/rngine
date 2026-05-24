import { Image } from 'react-native';
import { gameAssets } from './GameAssets.nitro';
import { gameMethods } from './GameMethods.nitro';
import type { Config, Entity, EntityUpdate } from './types';

const registerAssetIfNotExist = (assetId?: number) => {
  if (!assetId) {
    return;
  }

  if (gameAssets.isAssetRegistered(assetId)) {
    return;
  }

  const assetUri = Image.resolveAssetSource(assetId)?.uri;

  if (!assetUri) {
    return;
  }

  gameAssets.registerAsset(assetId, assetUri);
};

/** Sets up the game engine with the given configuration. Call this before anything else. */
export const configure = ({
  tickRate,
  screen,
  entities = [],
  systems = [],
  paused = true,
}: Config) => {
  registerAssetIfNotExist(screen.asset);
  for (const entity of entities) {
    registerAssetIfNotExist(entity.asset);
  }

  gameMethods.setTickRate(tickRate);
  gameMethods.setScreen(screen);
  gameMethods.setEntities(entities);
  gameMethods.setSystems(systems);
  if (paused) {
    gameMethods.pause();
  } else {
    gameMethods.resume();
  }
};

/** Spawns one or more entities into the world. Skips duplicates by id. */
export const spawn = (entities: Entity | Entity[]) => {
  const entityArray = Array.isArray(entities) ? entities : [entities];

  for (const entity of entityArray) {
    registerAssetIfNotExist(entity.asset);
  }

  gameMethods.spawn(entityArray);
};

/** Updates one or more entities. Only the provided fields are changed. */
export const update = (updates: EntityUpdate | EntityUpdate[]) => {
  const entityUpdateArray = Array.isArray(updates) ? updates : [updates];

  for (const entityUpdate of entityUpdateArray) {
    registerAssetIfNotExist(entityUpdate.asset);
  }

  gameMethods.update(Array.isArray(updates) ? updates : [updates]);
};
