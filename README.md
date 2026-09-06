# Rngine

A React Native game engine for building 2D games, powered by [Nitro Modules](https://nitro.margelo.com/) for high-performance C++ game logic and rendered natively with [Skia](https://skia.org/).

## Installation

```sh
npm install rngine react-native-nitro-modules
```

> `react-native-nitro-modules` is required as this library relies on [Nitro Modules](https://nitro.margelo.com/).

## Usage

```tsx
import { GameEngine, configure, update } from 'rngine';

configure({
  world: { tickRate: 60 },
  screen: { width: 800, height: 800, color: '#1a1a1a' },
  entities: [
    {
      id: 'player',
      px: 420,
      py: 420,
      color: '#0f0',
      shape: { radius: 20 },
    },
    {
      id: 'enemy_1',
      px: 120,
      py: 120,
      color: '#f00',
      shape: { width: 40, height: 40 },
      vx: 1000,
    },
    {
      id: 'enemy_2',
      px: 220,
      py: 220,
      color: '#f00',
      shape: { width: 40, height: 40 },
      vx: -1000,
    },
  ],
  systems: [
    {
      // runs every tick for all enemies
      entities: ['enemy'],
      onTick: (enemies) => {
        enemies.forEach((enemy) => {
          // reverse direction when reaching screen edges
          const halfWidth = enemy.shape.width / 2;
          if (enemy.px - halfWidth <= 0 || enemy.px + halfWidth >= 800) {
            update({ id: enemy.id, vx: -enemy.vx });
          }
        });
      },
    },
  ],
  paused: false,
});

export default function App() {
  return <GameEngine style={{ flex: 1 }} />;
}
```

## Concepts

**Entities** are the objects in your game world. Each entity has a position, shape, and optional color/asset/velocity/acceleration/mass. See [`Entity`](./src/nativeTypes.ts).

**Shapes** describe an entity's collision and rendering geometry: either a rectangle (`{ width, height }`) or a circle (`{ radius }`).

**Systems** define your game logic. Each system optionally declares which entities it cares about via `entities`, which collision pairs to watch via `collisions`, or both. Systems run every tick receiving the resolved entities and any active collisions. See [`System`](./src/types.ts).

**Screen** defines the viewport dimensions and optional background: color/asset. Entities outside the screen bounds are automatically clipped. See [`Screen`](./src/nativeTypes.ts)

**World** defines global simulation settings: `tickRate` (game logic updates per second) and optional gravity (`gx`/`gy`, in game units per second²). See [`World`](./src/nativeTypes.ts)

## Rendering

Rngine renders natively with [Skia](https://skia.org/) — every frame is drawn directly in C++ on the game loop thread, with no bridge round-trip per frame. SVG and [Lottie](https://airbnb.io/lottie/) animations are both supported as entity/screen assets, decoded and cached natively.

## Physics

Entities with a `mass` are affected by the world's gravity (`world.gx`/`gy`) and participate in automatic collision resolution: overlapping entities are pushed apart and have their velocities updated via an impulse response. Entities without a `mass` (or `mass: 0`) are treated as immovable (infinite mass) when involved in a collision — useful for static geometry like a ground or walls.

Set `isSensor: true` on an entity to still receive collision events (`Collision.depth`, `Collision.nx`/`ny`) without any physical response — the entity won't be pushed, and other entities won't be pushed by it. This is useful for pickups, triggers, or grid-based games where you want to detect overlap yourself without physics involved.

## Entity Querying

Entity ids use `_` as a hierarchy separator, which enables prefix matching in systems and functions like `update`. Querying `'enemy'` matches all entities whose id starts with `enemy_`. So `enemy_1`, `enemy_magician_1`, and `enemy_warrior_2` are all returned. Querying `'enemy_magician'` narrows it down to only magician entities. Querying `'enemy_magician_1'` is an exact match.

This makes it easy to build entity groups naturally through naming. No extra configuration needed.

```ts
// matches enemy_1, enemy_magician_1, enemy_warrior_2
{ entities: ['enemy'], onTick: (entities) => {} }

// matches only enemy_magician_1, enemy_magician_2
{ entities: ['enemy_magician'], onTick: (entities) => {} }

// exact match
{ entities: ['enemy_magician_1'], onTick: (entities) => {} }

// collision detection between the player and all enemies
{ collisions: [{ a: 'player', b: 'enemy' }], onTick: (_, collisions) => {} }
```

Collision results include a normalized direction vector (`nx`/`ny`) pointing along the minimum separating axis, alongside the penetration `depth` — useful for resolving collisions manually or reacting to a specific direction of impact.

## Assets

Assets (SVGs and Lottie animations) must be preloaded with `loadAssets` before being referenced by `id`, `px`/`py`, or any `configure`/`spawn`/`update` call. Loading happens off the JS thread and is cached — calling `loadAssets` again with the same source is a no-op.

```ts
import { loadAssets } from 'rngine';

const assets = await loadAssets({
  player: require('./assets/player.svg'),
  explosion: require('./assets/explosion.json'), // Lottie
});

// assets.player and assets.explosion are numeric ids, ready to use as `asset` on any entity or the screen
```

## API

### `configure(config)`

Sets up the game engine. Call this before anything else.

| param      | required | type       | default | description                                   |
| ---------- | -------- | ---------- | ------- | --------------------------------------------- |
| `world`    | ✓        | `World`    | -       | Tick rate and optional gravity                |
| `screen`   | ✓        | `Screen`   | -       | Screen dimensions, background color and asset |
| `entities` |          | `Entity[]` | `[]`    | Initial entities to spawn                     |
| `systems`  |          | `System[]` | `[]`    | Systems to run each tick                      |
| `paused`   |          | `boolean`  | `true`  | Whether to start paused                       |

### `loadAssets(assets)`

Preloads one or more SVG or Lottie assets ahead of time. Takes an object mapping names to `require(...)` sources (SVG) or imported JSON (Lottie), and resolves to an object of the same shape with numeric asset ids.

### `spawn(entity | entity[])`

Spawns one or more entities into the world. Skips duplicates by id.

### `despawn(id)`

Removes the entity with the given id, or all entities matching the given prefix.

### `update(entityUpdate | entityUpdate[])`

Updates one or more entities. Only the provided fields are changed.
Matches entities by exact id or prefix.

### `pause()`

Pauses the game loop. Systems stop running.

### `resume()`

Resumes the game loop.

## Contributing

- [Development workflow](CONTRIBUTING.md#development-workflow)
- [Sending a pull request](CONTRIBUTING.md#sending-a-pull-request)
- [Code of conduct](CODE_OF_CONDUCT.md)

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
