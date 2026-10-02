import {
  pause,
  spawn,
  updateEntities,
  type EntityUpdate,
  type System,
} from 'rngine';
import { setGameState, snakeState } from './state';
import { CELL, COLS, HALF_CELL, ROWS } from './consts';
import { getAssets } from '../../assets/assetsManager';

const {
  Head_Up,
  Head_Left,
  Body_Up_Left,
  Body_Horizontal,
  Body_Vertical,
  Tail_Up,
  Tail_Right,
} = getAssets('Snake') ?? {};

export const systems: System[] = [
  {
    entities: ['food', 'snake_head', 'snake_body'],
    onTick: ({ entities }) => {
      const updates: (Omit<EntityUpdate, 'px' | 'py'> & {
        px: number;
        py: number;
      })[] = [
        {
          id: entities[1]!.id,
          px: entities[1]!.px,
          py: entities[1]!.py,
        },
      ];

      switch (snakeState.direction) {
        case 'UP':
          updates[0]!.py -= CELL;
          break;
        case 'DOWN':
          updates[0]!.py += CELL;
          break;
        case 'LEFT':
          updates[0]!.px -= CELL;
          break;
        case 'RIGHT':
          updates[0]!.px += CELL;
          break;
      }

      if (
        updates[0]!.px < 0 ||
        updates[0]!.py < 0 ||
        updates[0]!.px + CELL > CELL * COLS + HALF_CELL ||
        updates[0]!.py + CELL > CELL * ROWS + HALF_CELL
      ) {
        pause();
        setGameState('OVER');
        return;
      }

      for (let i = 2; i < entities.length; i++) {
        if (
          updates[0]!.px === entities[i - 1]!.px &&
          updates[0]!.py === entities[i - 1]!.py
        ) {
          pause();
          setGameState('OVER');
          return;
        }

        updates.push({
          id: entities[i]!.id,
          px: entities[i - 1]!.px,
          py: entities[i - 1]!.py,
        });
      }

      if (
        entities[1]!.px === entities[0]!.px &&
        entities[1]!.py === entities[0]!.py
      ) {
        updates.push({
          id: 'food',
          px: Math.floor(Math.random() * COLS) * CELL + HALF_CELL,
          py: Math.floor(Math.random() * ROWS) * CELL + HALF_CELL,
        });

        spawn({
          id: `snake_body_${
            entities.length - 1 < 10
              ? '00'
              : entities.length - 1 < 100
                ? '0'
                : ''
          }${entities.length - 1}`,
          px: entities[entities.length - 1]!.px,
          py: entities[entities.length - 1]!.py,
          shape: { width: CELL, height: CELL },
          isSensor: true,
        });
      }
      updateEntities(updates);
    },
  },
  {
    entities: ['snake_head', 'snake_body'],
    onTick: ({ entities }) => {
      const updates: EntityUpdate[] = [];
      const HEAD_ASSET_MAP = {
        UP: { asset: Head_Up, flipH: false, flipV: false },
        DOWN: { asset: Head_Up, flipH: false, flipV: true },
        LEFT: { asset: Head_Left, flipH: false, flipV: false },
        RIGHT: { asset: Head_Left, flipH: true, flipV: false },
      };

      updates.push({
        id: entities[0]!.id,
        ...HEAD_ASSET_MAP[snakeState.direction],
      });

      const BODY_ASSET_MAP = {
        LEFT_UP: { asset: Body_Up_Left, flipH: false, flipV: false },
        DOWN_RIGHT: {
          asset: Body_Up_Left,
          flipH: true,
          flipV: true,
        },
        DOWN_LEFT: {
          asset: Body_Up_Left,
          flipH: false,
          flipV: true,
        },
        RIGHT_UP: { asset: Body_Up_Left, flipH: true, flipV: false },
        HORIZONTAL: {
          asset: Body_Horizontal,
          flipH: false,
          flipV: false,
        },
        VERTICAL: {
          asset: Body_Vertical,
          flipH: false,
          flipV: false,
        },
      };

      let bodyKey: keyof typeof BODY_ASSET_MAP;

      for (let i = 1; i < entities.length - 1; i++) {
        const previousPx = entities[i - 1]!.px;
        const previousPy = entities[i - 1]!.py;
        const currentPx = entities[i]!.px;
        const currentPy = entities[i]!.py;
        const nextPx = entities[i + 1]!.px;
        const nextPy = entities[i + 1]!.py;

        if (previousPx === currentPx && currentPx === nextPx) {
          bodyKey = 'VERTICAL';
        } else if (previousPy === currentPy && currentPy === nextPy) {
          bodyKey = 'HORIZONTAL';
        } else {
          const dirTowardHead =
            previousPx > currentPx
              ? 'RIGHT'
              : previousPx < currentPx
                ? 'LEFT'
                : previousPy > currentPy
                  ? 'DOWN'
                  : 'UP';

          const dirTowardTail =
            nextPx > currentPx
              ? 'RIGHT'
              : nextPx < currentPx
                ? 'LEFT'
                : nextPy > currentPy
                  ? 'DOWN'
                  : 'UP';

          const pair = [dirTowardHead, dirTowardTail].sort().join('_') as
            'LEFT_UP' | 'DOWN_RIGHT' | 'DOWN_LEFT' | 'RIGHT_UP';

          bodyKey = pair;
        }

        updates.push({
          id: entities[i]!.id,
          ...BODY_ASSET_MAP[bodyKey],
        });
      }

      const TAIL_ASSET_MAP = {
        UP: { asset: Tail_Up, flipH: false, flipV: false },
        DOWN: { asset: Tail_Up, flipH: false, flipV: true },
        LEFT: { asset: Tail_Right, flipH: true, flipV: false },
        RIGHT: { asset: Tail_Right, flipH: false, flipV: false },
      };

      let tailKey: keyof typeof TAIL_ASSET_MAP;

      if (
        entities[entities.length - 1]!.px === entities[entities.length - 2]!.px
      ) {
        if (
          entities[entities.length - 1]!.py > entities[entities.length - 2]!.py
        ) {
          tailKey = 'DOWN';
        } else {
          tailKey = 'UP';
        }
      } else {
        if (
          entities[entities.length - 1]!.px > entities[entities.length - 2]!.px
        ) {
          tailKey = 'RIGHT';
        } else {
          tailKey = 'LEFT';
        }
      }
      updates.push({
        id: entities[entities.length - 1]!.id,
        ...TAIL_ASSET_MAP[tailKey],
      });

      updateEntities(updates);

      return;
    },
  },
];
