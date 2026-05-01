import { NitroModules } from 'react-native-nitro-modules';
import type { GameMethods } from './GameMethods.nitro';
import type { Entity, System, World } from './types';

const gameMethods = NitroModules.createHybridObject<GameMethods>('GameMethods');

/** Sets up the game engine with the given configuration. Call this before anything else. */
export function configure({
  tickRate,
  world,
  entities = [],
  systems = [],
  paused = true,
}: {
  /** Number of game logic updates per second. */
  tickRate: number;
  /** World dimensions and background color. */
  world: World;
  /** Initial entities to spawn. */
  entities?: Entity[];
  /**
   * Systems that define the game logic. Each system runs every tick
   * and receives the entities matching its ids.
   */
  systems?: System[];
  /** Whether to start paused. Defaults to true. */
  paused?: boolean;
}) {
  gameMethods.setTickRate(tickRate);
  gameMethods.setWorld(world);
  gameMethods.setEntities(entities);
  gameMethods.setSystems(systems);
  if (!paused) {
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

/** Sets the position of the entity with the exact given id. */
export const setPosition = (id: string, px: number, py: number) =>
  gameMethods.setP(id, px, py);

/** Sets the velocity of all entities matching the given id or prefix. */
export const setVelocity = (id: string, vx: number, vy: number) =>
  gameMethods.setV(id, vx, vy);
