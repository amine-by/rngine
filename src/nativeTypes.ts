interface Positioned {
  /** X position in game units. */
  px: number;
  /** Y position in game units. */
  py: number;
}

interface Rect {
  /** Width in game units. */
  width: number;
  /** Height in game units. */
  height: number;
}

interface Circle {
  /** Radius in game units. */
  radius: number;
}

interface Shaped {
  /** Shape of the entity. */
  shape: Rect | Circle;
  /** Indicates whether the entity is a sensor. Sensors trigger collision events but don't react physically. */
  isSensor?: boolean;
}

interface Renderable {
  /** Fill color as a hex string e.g. `'#00ff00'`. */
  color?: string;
  /** Asset to render, use `require` with the file path to get the id. */
  asset?: number;
  /** Animation progress between 0 and 1, automatically advanced each tick for Lottie assets. */
  progress?: number;
}

interface Kinematic {
  /** Horizontal velocity in game units per second. */
  vx?: number;
  /** Vertical velocity in game units per second. */
  vy?: number;
  /** Horizontal acceleration in game units per second squared. */
  ax?: number;
  /** Vertical acceleration in game units per second squared. */
  ay?: number;
  /** Mass in arbitrary units. */
  mass?: number;
}

export interface World {
  /** Number of game logic updates per second. */
  tickRate: number;
  /** Horizontal gravitational acceleration in game units per second squared. */
  gx?: number;
  /** Vertical gravitational acceleration in game units per second squared. */
  gy?: number;
}

export interface Screen extends Rect, Renderable {}

export interface Entity extends Positioned, Shaped, Renderable, Kinematic {
  /** Unique identifier. Use `_` as a separator for group queries e.g. `'enemy_1'`. */
  id: string;
}

export interface EntityUpdate extends Partial<
  Positioned & Shaped & Renderable & Kinematic
> {
  /** Unique identifier or prefix of the entities to update. */
  id: string;
}

export interface CollisionPair {
  /** Entity id or prefix of the first entity to watch. */
  a: string;
  /** Entity id or prefix of the second entity to watch. */
  b: string;
}

export interface Collision {
  /** Id of the first entity involved in the collision. */
  a: string;
  /** Id of the second entity involved in the collision. */
  b: string;
  /** Penetration depth of the collision. */
  depth: number;
  /** X component of a normalised vector that represents the direction between the bodies providing the minimum separating distance. */
  nx: number;
  /** Y component of a normalised vector that represents the direction between the bodies providing the minimum separating distance. */
  ny: number;
}

export interface System {
  /** Entity ids or prefixes this system subscribes to. */
  entities?: string[];
  /** Collision pairs to watch for every tick. */
  collisions?: CollisionPair[];
  /** Called every tick with the resolved entities and collisions. */
  onTick: (entities: Entity[], collisions: Collision[]) => number;
}
