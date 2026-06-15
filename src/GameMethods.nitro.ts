import type { HybridObject } from 'react-native-nitro-modules';
import type { Entity, EntityUpdate, System, Screen } from './nativeTypes';
import { NitroModules } from 'react-native-nitro-modules';

interface GameMethods extends HybridObject<{
  ios: 'c++';
  android: 'c++';
}> {
  setTickRate(tickRate: number): void;
  setScreen(screen: Screen): void;
  setEntities(entities: Entity[]): void;
  setSystems(systems: System[]): void;
  pause(): void;
  resume(): void;
  spawn(entities: Entity[]): void;
  despawn(id: string): void;
  update(updates: EntityUpdate[]): void;
}

export const gameMethods =
  NitroModules.createHybridObject<GameMethods>('GameMethods');
