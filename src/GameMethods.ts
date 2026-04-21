import { NitroModules } from 'react-native-nitro-modules';
import type { GameMethods } from './GameMethods.nitro';

const gameMethods = NitroModules.createHybridObject<GameMethods>('GameMethods');

export const setP = (id: string, px: number, py: number) => {
  gameMethods?.setP(id, px, py);
};

export const setV = (id: string, vx: number, vy: number) => {
  gameMethods?.setV(id, vx, vy);
};
