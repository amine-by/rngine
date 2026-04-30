import type { HybridObject } from 'react-native-nitro-modules';
import type { Entity, System, World } from './types';

export interface GameMethods extends HybridObject<{
  ios: 'c++';
  android: 'c++';
}> {
  setTickRate(tickRate: number): void;
  setWorld(world: World): void;
  setEntities(entities: Entity[]): void;
  setSystems(systems: System[]): void;
  pause(): void;
  resume(): void;
  spawn(entities: Entity[]): void;
  despawn(id: string): void;
  setP(id: string, px: number, py: number): void;
  setV(id: string, vx: number, vy: number): void;
}
