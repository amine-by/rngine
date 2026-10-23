import type { HybridObject } from 'react-native-nitro-modules';
import type {
  EntityUpdate,
  System,
  ScreenUpdate,
  WorldInitializer,
  ScreenInitializer,
  EntityInitializer,
} from './types';
import { NitroModules } from 'react-native-nitro-modules';

interface GameMethods extends HybridObject<{
  ios: 'c++';
  android: 'c++';
}> {
  /** Configuration Methods */
  setWorld(world: WorldInitializer): void;
  setScreen(screen: ScreenInitializer): void;
  setEntities(entities: EntityInitializer[]): void;
  setSystems(systems: System[]): void;

  /** Pause/Resume Methods */
  pause(): void;
  resume(): void;

  /** Entity Update Methods */
  spawn(entities: EntityInitializer[]): void;
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
