import type {
  System as NativeSystem,
  Collision,
  Screen,
  Entity,
} from './nativeTypes';

export type System = Omit<NativeSystem, 'onTick'> & {
  /** Called every tick with the resolved entities and collisions. */
  onTick: (entities: Entity[], collisions: Collision[]) => void;
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
