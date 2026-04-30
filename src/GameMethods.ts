import { NitroModules } from 'react-native-nitro-modules';
import type { GameMethods } from './GameMethods.nitro';
import type { Entity, System, World } from './types';

const gameMethods = NitroModules.createHybridObject<GameMethods>('GameMethods');

export function configure({
  tickRate,
  world,
  entities = [],
  systems = [],
  paused = true,
}: {
  tickRate: number;
  world: World;
  entities?: Entity[];
  systems?: System[];
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

export const pause = () => gameMethods.pause();
export const resume = () => gameMethods.resume();
export const spawn = (entities: Entity | Entity[]) => {
  gameMethods.spawn(Array.isArray(entities) ? entities : [entities]);
};
export const despawn = (id: string) => gameMethods.despawn(id);
export const setPosition = (id: string, px: number, py: number) =>
  gameMethods.setP(id, px, py);
export const setVelocity = (id: string, vx: number, vy: number) =>
  gameMethods.setV(id, vx, vy);
