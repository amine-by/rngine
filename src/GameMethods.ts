import { gameMethods } from './GameMethods.nitro';
import type { Entity, EntityUpdate, ScreenUpdate, System } from './nativeTypes';
import type { Config } from './types';
import { Image } from 'react-native';

/** Sets up the game engine with the given configuration. Call this before anything else. */
export const configure = ({
  world,
  screen,
  entities = [],
  systems = [],
  paused = true,
}: Config) => {
  let nativeSystems: System[] = [];

  for (const system of systems) {
    nativeSystems.push({
      entities: system.entities,
      collisions: system.collisions,
      onTick: (systemContext) => {
        const start = Date.now();
        system.onTick(systemContext);
        const finish = Date.now();
        return finish - start;
      },
    });
  }

  gameMethods.setWorld(world);
  gameMethods.setScreen(screen);
  gameMethods.setEntities(entities);
  gameMethods.setSystems(nativeSystems);
  if (paused) {
    gameMethods.pause();
  } else {
    gameMethods.resume();
  }
};

/** Pauses the game loop. Systems will stop running. */
export const pause = () => gameMethods.pause();

/** Resumes the game loop. */
export const resume = () => gameMethods.resume();

/** Spawns one or more entities into the world. Skips duplicates by id. */
export const spawn = (entities: Entity | Entity[]) => {
  const entityArray = Array.isArray(entities) ? entities : [entities];
  gameMethods.spawn(entityArray);
};

/** Removes an entity or all entities matching the given id prefix from the world. Only the provided fields are changed. */
export const despawn = (id: string) => gameMethods.despawn(id);

/** Fires one update, applied to entities matching the given id prefix from the world. Only the provided fields are changed. */
export const updateEntity = (entityUpdate: EntityUpdate) => {
  gameMethods.updateEntities([entityUpdate]);
};

/** Fires multiple updates, each applied to entities matching the given id prefix from the world. */
export const updateEntities = (entityUpdates: EntityUpdate[]) => {
  gameMethods.updateEntities(entityUpdates);
};

/** Updates the screen. Only the provided fields are changed. */
export const updateScreen = (screenUpdate: ScreenUpdate) => {
  gameMethods.updateScreen(screenUpdate);
};

const assetCache = new WeakMap<object, number>();
let nextId = -1;

const loadAsset = async (asset: unknown) => {
  switch (typeof asset) {
    case 'object':
      if (asset === null) {
        throw new Error('loadAssets: asset cannot be null');
      }
      if (assetCache.has(asset)) {
        return assetCache.get(asset)!;
      }
      const assetId = nextId--;
      assetCache.set(asset, assetId);
      const isLottieLoaded = await gameMethods.loadLottie(
        assetId,
        JSON.stringify(asset)
      );
      if (!isLottieLoaded) {
        throw new Error('loadAssets: Failed to load lottie asset');
      }
      return assetId;

    case 'number':
      if (gameMethods.isAssetLoaded(asset)) {
        return asset;
      }
      const assetUri = Image.resolveAssetSource(asset)?.uri;
      if (!assetUri) {
        throw new Error(`loadAssets: could not resolve asset URI for ${asset}`);
      }
      const isLoaded = await gameMethods.loadImage(asset, assetUri);
      if (!isLoaded) {
        throw new Error('loadAssets: Failed to load image asset');
      }
      return asset;
  }

  throw new Error(`loadAssets: unsupported asset type "${typeof asset}"`);
};

/** Preloads one or more SVG, PNG or Lottie assets ahead of time.
 * Takes an object mapping names to `require(...)` sources (SVG/PNG) or imported JSON (Lottie),
 * and resolves to an object of the same shape with numeric asset ids. */
export const loadAssets = async <T extends Record<string, unknown>>(
  assets: T
): Promise<{ [K in keyof T]: number }> => {
  const keys = Object.keys(assets) as (keyof T)[];
  const result = {} as { [K in keyof T]: number };

  for (const key of keys) {
    result[key] = await loadAsset(assets[key]);
  }

  return result;
};
