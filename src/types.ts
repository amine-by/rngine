import type {
  System as NativeSystem,
  Screen,
  Entity,
  World,
  SystemContext,
} from './nativeTypes';

export type System = Omit<NativeSystem, 'onTick'> & {
  /** Called every tick with the resolved entities and collisions. */
  onTick: (systemContext: SystemContext) => void;
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
