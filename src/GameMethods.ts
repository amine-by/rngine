import { NitroModules } from 'react-native-nitro-modules';
import type { GameMethods } from './GameMethods.nitro';

const gameMethods = NitroModules.createHybridObject<GameMethods>('GameMethods');

export const setV = (id: string, vx: number, vy: number) => {
  gameMethods?.setV(id, vx, vy);
};
