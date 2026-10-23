import type { CustomType } from 'react-native-nitro-modules';

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
  /** Rotation angle in radians. */
  angle?: number;
  /** Indicates whether the entity is a sensor. Sensors trigger collision events but don't react physically. */
  isSensor?: boolean;
}

export type ObjectFit = 'fill' | 'contain' | 'cover' | 'none';

export type Repeat = 'no-repeat' | 'repeat' | 'repeat-x' | 'repeat-y';

interface Renderable {
  /** Fill color as a hex string e.g. `'#00ff00'`. */
  color?: string;
  /** Asset to render, use `require` with the file path to get the id. */
  asset?: number;
  /** Whether to flip the rendered asset horizontally. */
  flipH?: boolean;
  /** Whether to flip the rendered asset vertically. */
  flipV?: boolean;
  /** How the asset should be scaled to fit its target dimensions. Defaults to `'fill'`. */
  objectFit?: ObjectFit;
  /** Explicitly clips content that overflows the target dimensions. Defaults to `false`. */
  clip?: boolean;
  /** Repeating/tiling mode for the asset across target bounds. Defaults to `'no-repeat'`. */
  repeat?: Repeat;
  /** Animation progress between 0 and 1, automatically advanced each tick for Lottie assets. */
  progress?: number;
  /** Playback speed multiplier for Lottie assets (1 = normal speed). */
  speed?: number;
  /** Whether the Lottie animation should restart automatically upon completion. */
  loop?: boolean;
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

export interface WorldInitializer {
  /** Number of game logic updates per second. */
  tickRate: number;
  /** Horizontal gravitational acceleration in game units per second squared. */
  gx?: number;
  /** Vertical gravitational acceleration in game units per second squared. */
  gy?: number;
}

export type World = CustomType<
  Required<WorldInitializer>,
  'margelo::nitro::rngine::World',
  { include: 'World.hpp' }
>;

export interface ScreenInitializer extends Positioned, Rect, Renderable {}

export type Screen = CustomType<
  Required<ScreenInitializer>,
  'margelo::nitro::rngine::Screen',
  { include: 'Screen.hpp' }
>;

export interface ScreenUpdate extends Partial<ScreenInitializer> {}

export interface EntityInitializer
  extends Positioned, Shaped, Renderable, Kinematic {
  /** Unique identifier. Use `_` as a separator for group queries e.g. `'enemy_1'`. */
  id: string;
}

export type Entity = CustomType<
  Required<EntityInitializer>,
  'margelo::nitro::rngine::Entity',
  { include: 'Entity.hpp' }
>;

export interface EntityUpdate extends Partial<
  Positioned & Shaped & Renderable & Kinematic
> {
  /** Unique identifier or prefix of the entities to update. */
  id: string;
}

export type CollisionPair = CustomType<
  {
    /** Entity id or prefix of the first entity to watch. */
    a: string;
    /** Entity id or prefix of the second entity to watch. */
    b: string;
  },
  'margelo::nitro::rngine::CollisionPair',
  { include: 'CollisionPair.hpp' }
>;

export type Collision = CustomType<
  {
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
  },
  'margelo::nitro::rngine::Collision',
  { include: 'Collision.hpp' }
>;

export type SystemContext = CustomType<
  {
    /** Current game world state. */
    world?: World;
    /** Current game screen state. */
    screen?: Screen;
    /** Entities resolved from the system's entity subscriptions. */
    entities?: Entity[];
    /** Collisions matching the system's collision subscriptions. */
    collisions?: Collision[];
  },
  'margelo::nitro::rngine::SystemContext',
  { include: 'SystemContext.hpp' }
>;

export type System = CustomType<
  {
    /** Whether to include the world state in the system context. */
    world?: boolean;
    /** Whether to include the screen state in the system context. */
    screen?: boolean;
    /** Entity ids or prefixes this system subscribes to. */
    entities?: string[];
    /** Collision pairs to watch for every tick. */
    collisions?: CollisionPair[];
    /** Called every tick with the resolved entities and collisions. */
    onTick: (systemContext: SystemContext) => void;
  },
  'margelo::nitro::rngine::System',
  { include: 'System.hpp' }
>;

export type Config = {
  /** World settings: tick rate and gravity. */
  world: WorldInitializer;
  /** Screen dimensions and background. */
  screen: ScreenInitializer;
  /** Entities to spawn when the game starts. */
  entities?: EntityInitializer[];
  /** Systems that define game logic, each running every tick against its subscribed entities and collisions. */
  systems?: System[];
  /** Whether the game starts paused. Defaults to true. */
  paused?: boolean;
};
