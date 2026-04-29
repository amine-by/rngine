import type { HybridObject } from 'react-native-nitro-modules';
import type { Entity, System, World } from './types';

export interface GameMethods extends HybridObject<{
  ios: 'c++';
  android: 'c++';
}> {
  initialize(
    isPaused: boolean,
    tickRate: number,
    world: World,
    entities: Entity[],
    systems: System[]
  ): void;
  pause(): void;
  resume(): void;
  spawn(entity: Entity): void;
  despawn(id: string): void;
  setP(id: string, px: number, py: number): void;
  setV(id: string, vx: number, vy: number): void;
}
