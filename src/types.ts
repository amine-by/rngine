import type {
  System as NativeSystem,
  Collision,
  Screen,
  Entity,
  World,
} from './nativeTypes';

export type System = Omit<NativeSystem, 'onTick'> & {
  /** Called every tick with the resolved entities and collisions. */
  onTick: (entities: Entity[], collisions: Collision[]) => void;
};

export type Config = {
  /** World settings: tick rate and gravity. */
  world: World;
  /** Screen dimensions and background. */
  screen: Screen;
  /** Entities to spawn when the game starts. */
  entities?: Entity[];
  /** Systems that define game logic, each running every tick against its subscribed entities and collisions. */
  systems?: System[];
  /** Whether the game starts paused. Defaults to true. */
  paused?: boolean;
};
