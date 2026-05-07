import { NitroModules } from 'react-native-nitro-modules';
import type { GameMethods } from './GameMethods.nitro';
import type { Config, Entity, EntityUpdate } from './types';

const gameMethods = NitroModules.createHybridObject<GameMethods>('GameMethods');

/** Sets up the game engine with the given configuration. Call this before anything else. */
export function configure({
  tickRate,
  world,
  entities = [],
  systems = [],
  paused = true,
}: Config) {
  gameMethods.setTickRate(tickRate);
  gameMethods.setWorld(world);
  gameMethods.setEntities(entities);
  gameMethods.setSystems(systems);
  if (paused) {
    gameMethods.pause();
  } else {
    gameMethods.resume();
  }
}

/** Pauses the game loop. Systems will stop running. */
export const pause = () => gameMethods.pause();

/** Resumes the game loop. */
export const resume = () => gameMethods.resume();

/** Spawns one or more entities into the world. Skips duplicates by id. */
export const spawn = (entities: Entity | Entity[]) => {
  gameMethods.spawn(Array.isArray(entities) ? entities : [entities]);
};

/** Removes an entity or all entities matching the given id prefix from the world. */
export const despawn = (id: string) => gameMethods.despawn(id);

export const update = (updates: EntityUpdate | EntityUpdate[]) => {
  gameMethods.update(Array.isArray(updates) ? updates : [updates]);
};
