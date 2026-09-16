import type { HybridObject } from 'react-native-nitro-modules';
import type {
  Entity,
  EntityUpdate,
  System,
  Screen,
  World,
  ScreenUpdate,
} from './nativeTypes';
import { NitroModules } from 'react-native-nitro-modules';

interface GameMethods extends HybridObject<{
  ios: 'c++';
  android: 'c++';
}> {
  /** Configuration Methods */
  setWorld(world: World): void;
  setScreen(screen: Screen): void;
  setEntities(entities: Entity[]): void;
  setSystems(systems: System[]): void;

  /** Pause/Resume Methods */
  pause(): void;
  resume(): void;

  /** Entity Update Methods */
  spawn(entities: Entity[]): void;
  despawn(id: string): void;
  updateEntities(entityUpdates: EntityUpdate[]): void;

  /** Screen Update Methods */
  updateScreen(screenUpdate: ScreenUpdate): void;

  /** Asset Methods */
  isAssetLoaded(id: number): boolean;
  loadLottie(id: number, jsonStr: string): Promise<boolean>;
  loadImage(id: number, imageUri: string): Promise<boolean>;
}

export const gameMethods =
  NitroModules.createHybridObject<GameMethods>('GameMethods');
