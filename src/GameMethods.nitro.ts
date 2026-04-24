import type { HybridObject } from 'react-native-nitro-modules';
import type { Entity } from './types';

export interface GameMethods extends HybridObject<{
  ios: 'c++';
  android: 'c++';
}> {
  initialize(isPaused: boolean, entities: Entity[]): void;
  pause(): void;
  resume(): void;
  setP(id: string, px: number, py: number): void;
  setV(id: string, vx: number, vy: number): void;
}
