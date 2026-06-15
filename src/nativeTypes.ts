export type Screen = {
  /** Width of the screen in game units. */
  width: number;
  /** Height of the screen in game units. */
  height: number;
  /** Background color of the screen as a hex string e.g. `'#1a1a1a'`. */
  color?: string;
  /** Asset to render, use `require` with the file path to get the id. */
  asset?: number;
  /** Animation progress between 0 and 1, automatically advanced each tick for Lottie assets. */
  progress?: number;
};

export type Entity = {
  /** Unique identifier. Use `_` as a separator for group queries e.g. `'enemy_1'`. */
  id: string;
  /** X position in game units. */
  px: number;
  /** Y position in game units. */
  py: number;
  /** Width in game units. */
  width: number;
  /** Height in game units. */
  height: number;
  /** Fill color as a hex string e.g. `'#00ff00'`. */
  color?: string;
  /** Asset to render, use `require` with the file path to get the id. */
  asset?: number;
  /** Animation progress between 0 and 1, automatically advanced each tick for Lottie assets. */
  progress?: number;
  /** Velocity on the X axis in game units per second. */
  vx?: number;
  /** Velocity on the Y axis in game units per second. */
  vy?: number;
};

export type System = {
  /** Entity ids or prefixes this system subscribes to. */
  ids: string[];
  /** Called every tick with the resolved entities. */
  onTick: (entities: Entity[]) => void;
};

export type EntityUpdate = {
  /** Unique identifier or prefix of the entities to update. */
  id: string;
  /** X position in game units. */
  px?: number;
  /** Y position in game units. */
  py?: number;
  /** Width in game units. */
  width?: number;
  /** Height in game units. */
  height?: number;
  /** Fill color as a hex string e.g. `'#00ff00'`. */
  color?: string;
  /** Asset to render, use `require` with the file path to get the id. */
  asset?: number;
  /** Animation progress between 0 and 1, automatically advanced each tick for Lottie assets. */
  progress?: number;
  /** Velocity on the X axis in game units per second. */
  vx?: number;
  /** Velocity on the Y axis in game units per second. */
  vy?: number;
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
