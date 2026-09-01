import { gameMethods } from './GameMethods.nitro';
import type { Entity, EntityUpdate, System } from './nativeTypes';
import type { Config } from './types';

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
      onTick: (e, c) => {
        const start = Date.now();
        system.onTick(e, c);
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

/** Removes an entity or all entities matching the given id prefix from the world. */
export const despawn = (id: string) => gameMethods.despawn(id);

/** Updates one or more entities. Only the provided fields are changed. */
export const update = (updates: EntityUpdate | EntityUpdate[]) => {
  const entityUpdateArray = Array.isArray(updates) ? updates : [updates];
  gameMethods.update(entityUpdateArray);
};
