import type {
  Screen as NativeScreen,
  Entity as NativeEntity,
  EntityUpdate as NativeEntityUpdate,
  System,
} from './nativeTypes';

export type Asset = number | object;

export type Screen = Omit<NativeScreen, 'asset'> & {
  /** Asset to render, use `require` with the file path to get the id. */
  asset?: Asset;
};
export type Entity = Omit<NativeEntity, 'asset'> & {
  /** Asset to render, use `require` with the file path to get the id. */
  asset?: Asset;
};
export type EntityUpdate = Omit<NativeEntityUpdate, 'asset'> & {
  /** Asset to render, use `require` with the file path to get the id. */
  asset?: Asset;
};

export type Config = {
  /** Number of game logic updates per second. */
  tickRate: number;
  /** Screen dimensions and background color. */
  screen: Screen;
  /** Initial entities to spawn. */
  entities?: Entity[];
  /**
   * Systems that define the game logic. Each system runs every tick
   * and receives the entities matching its ids.
   */
  systems?: System[];
  /** Whether to start paused. Defaults to true. */
  paused?: boolean;
};
