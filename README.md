# rngine

A React Native game engine for building 2D games, powered by [Nitro Modules](https://nitro.margelo.com/) for high-performance C++ game logic.

## Installation

```sh
npm install rngine react-native-nitro-modules
```

> `react-native-nitro-modules` is required as this library relies on [Nitro Modules](https://nitro.margelo.com/).

## Usage

```tsx
import { GameEngine, configure, setVelocity } from 'rngine';

configure({
  tickRate: 60,
  world: { width: 800, height: 800, color: '#1a1a1a' },
  entities: [
    {
      id: 'player',
      px: 400,
      py: 400,
      width: 40,
      height: 40,
      color: '#00ff00',
      vx: 0,
      vy: 0,
    },
    {
      id: 'enemy_1',
      px: 100,
      py: 100,
      width: 40,
      height: 40,
      color: '#ff0000',
      vx: 1000,
      vy: 0,
    },
    {
      id: 'enemy_2',
      px: 200,
      py: 200,
      width: 40,
      height: 40,
      color: '#ff0000',
      vx: -1000,
      vy: 0,
    },
  ],
  systems: [
    {
      // runs every tick for all enemies
      ids: ['enemy'],
      onTick: (enemies) => {
        enemies.forEach((enemy) => {
          // reverse direction when reaching world edges
          if (enemy.px <= 0 || enemy.px + enemy.width >= 800) {
            setVelocity(enemy.id, -enemy.vx, enemy.vy);
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

**Entities** are the objects in your game world. Each entity has a position, size, color, and velocity. See [`Entity`](./src/types.ts).

**Systems** define your game logic. Each system declares which entities it cares about via `ids` and runs every tick receiving those entities. See [`System`](./src/types.ts).

**World** defines the game world dimensions and background color. Entities outside the world bounds are automatically clipped. See [`World`](./src/types.ts).

## Entity Querying

Entity ids use `_` as a hierarchy separator, which enables prefix matching in systems and functions like `setVelocity`. Querying `'enemy'` matches all entities whose id starts with `enemy_`. So `enemy_1`, `enemy_magician_1`, and `enemy_warrior_2` are all returned. Querying `'enemy_magician'` narrows it down to only magician entities. Querying `'enemy_magician_1'` is an exact match.

This makes it easy to build entity groups naturally through naming. No extra configuration needed.

```ts
// matches enemy_1, enemy_magician_1, enemy_warrior_2
{ ids: ['enemy'], onTick: (entities) => {} }

// matches only enemy_magician_1, enemy_magician_2
{ ids: ['enemy_magician'], onTick: (entities) => {} }

// exact match
{ ids: ['enemy_magician_1'], onTick: (entities) => {} }
```

## API

### `configure(config)`

Sets up the game engine. Call this before anything else.

| param      | required | type       | default | description                           |
| ---------- | -------- | ---------- | ------- | ------------------------------------- |
| `tickRate` | ✓        | `number`   | —       | Game logic updates per second         |
| `world`    | ✓        | `World`    | —       | World dimensions and background color |
| `entities` |          | `Entity[]` | `[]`    | Initial entities to spawn             |
| `systems`  |          | `System[]` | `[]`    | Systems to run each tick              |
| `paused`   |          | `boolean`  | `true`  | Whether to start paused               |

### `spawn(entity | entity[])`

Spawns one or more entities into the world. Skips duplicates by id.

### `despawn(id)`

Removes the entity with the given id, or all entities matching the given prefix.

### `setPosition(id, px, py)`

Sets the position of a single entity. Requires an exact id.

### `setVelocity(id, vx, vy)`

Sets the velocity of all entities matching the given id or prefix.

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
