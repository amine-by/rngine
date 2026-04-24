import { NitroModules } from 'react-native-nitro-modules';
import type { GameMethods } from './GameMethods.nitro';
import type { Entity } from './types';

const gameMethods = NitroModules.createHybridObject<GameMethods>('GameMethods');

export const initialize = (isPaused: boolean, entities: Entity[]) => {
  gameMethods?.initialize(isPaused, entities);
};

export const pause = () => {
  gameMethods?.pause();
};

export const resume = () => {
  gameMethods?.resume();
};

export const spawn = (entity: Entity) => {
  gameMethods?.spawn(entity);
};

export const despawn = (id: string) => {
  gameMethods?.despawn(id);
};

export const setP = (id: string, px: number, py: number) => {
  gameMethods?.setP(id, px, py);
};

export const setV = (id: string, vx: number, vy: number) => {
  gameMethods?.setV(id, vx, vy);
};
