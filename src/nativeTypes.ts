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

export type CollisionPair = {
  /** Entity id or prefix of the first entity to watch. */
  a: string;
  /** Entity id or prefix of the second entity to watch. */
  b: string;
};

export type Collision = {
  /** Id of the first entity involved in the collision. */
  a: string;
  /** Id of the second entity involved in the collision. */
  b: string;
  /** Penetration depth of the collision. */
  depth: number;
};

export type System = {
  /** Entity ids or prefixes this system subscribes to. */
  entities?: string[];
  /** Collision pairs to watch for every tick. */
  collisions?: CollisionPair[];
  /** Called every tick with the resolved entities and collisions. */
  onTick: (entities: Entity[], collisions: Collision[]) => void;
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
