import { gameMethods } from './GameMethods.nitro';

/** Pauses the game loop. Systems will stop running. */
export const pause = () => gameMethods.pause();

/** Resumes the game loop. */
export const resume = () => gameMethods.resume();

/** Removes an entity or all entities matching the given id prefix from the world. */
export const despawn = (id: string) => gameMethods.despawn(id);
