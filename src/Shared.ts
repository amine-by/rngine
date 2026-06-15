import { Image } from 'react-native';
import { gameAssets } from './GameAssets.nitro';
import { gameMethods } from './GameMethods.nitro';
import type {
  Screen as NativeScreen,
  Entity as NativeEntity,
  EntityUpdate as NativeEntityUpdate,
} from './nativeTypes';
import type { Asset, Config, Entity, EntityUpdate } from './types';

const assetCache = new WeakMap<object, number>();
let nextId = -1;

const registerAssetIfNotExist = (asset?: Asset) => {
  switch (typeof asset) {
    case 'object':
      if (assetCache.has(asset)) {
        return assetCache.get(asset);
      }
      const assetId = nextId--;
      assetCache.set(asset, assetId);
      gameAssets.registerLottie(assetId, JSON.stringify(asset));
      return assetId;

    case 'number':
      if (gameAssets.isAssetRegistered(asset)) {
        return asset;
      }
      const assetUri = Image.resolveAssetSource(asset)?.uri;
      if (!assetUri) {
        break;
      }
      gameAssets.registerSvg(asset, assetUri);
      return asset;
  }
  return;
};

/** Sets up the game engine with the given configuration. Call this before anything else. */
export const configure = ({
  tickRate,
  screen,
  entities = [],
  systems = [],
  paused = true,
}: Config) => {
  screen.asset = registerAssetIfNotExist(screen.asset);
  for (const entity of entities) {
    entity.asset = registerAssetIfNotExist(entity.asset);
  }

  gameMethods.setTickRate(tickRate);
  gameMethods.setScreen(screen as NativeScreen);
  gameMethods.setEntities(entities as NativeEntity[]);
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
    entity.asset = registerAssetIfNotExist(entity.asset);
  }

  gameMethods.spawn(entityArray as NativeEntity[]);
};

/** Updates one or more entities. Only the provided fields are changed. */
export const update = (updates: EntityUpdate | EntityUpdate[]) => {
  const entityUpdateArray = Array.isArray(updates) ? updates : [updates];

  for (const entityUpdate of entityUpdateArray) {
    entityUpdate.asset = registerAssetIfNotExist(entityUpdate.asset);
  }

  gameMethods.update(entityUpdateArray as NativeEntityUpdate[]);
};
