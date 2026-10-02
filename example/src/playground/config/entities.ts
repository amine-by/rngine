import { getAssets } from '../../assets/assetsManager';
import {
  CEIL_HEIGHT,
  GROUND_HEIGHT,
  PLATFORM_HEIGHT,
  PLATFORM_WIDTH,
  SCREEN_HEIGHT,
  SCREEN_WIDTH,
  WALL_HEIGHT,
  WALL_WIDTH,
  WORLD_HEIGHT,
  WORLD_WIDTH,
} from './consts';

const { Appearing } = getAssets('Playground') ?? {};

export const entities = [
  {
    id: 'player',
    px: SCREEN_WIDTH / 2,
    py: WORLD_HEIGHT - GROUND_HEIGHT - 96 / 2,
    shape: { width: 96, height: 96 },
    asset: Appearing,
    speed: 1,
    loop: false,
    mass: 5,
  },
  {
    id: 'platform_ground',
    px: WORLD_WIDTH / 2,
    py: WORLD_HEIGHT - GROUND_HEIGHT / 2,
    shape: { width: WORLD_WIDTH, height: GROUND_HEIGHT },
    color: '#654321',
  },
  {
    id: 'ceil',
    px: WORLD_WIDTH / 2,
    py: CEIL_HEIGHT / 2,
    shape: { width: WORLD_WIDTH, height: CEIL_HEIGHT },
    color: '#654321',
  },
  {
    id: 'wall_left',
    px: WALL_WIDTH / 2,
    py: CEIL_HEIGHT + WALL_HEIGHT / 2,
    shape: {
      width: WALL_WIDTH,
      height: WALL_HEIGHT,
    },
    color: '#654321',
  },
  {
    id: 'wall_right',
    px: WORLD_WIDTH - WALL_WIDTH / 2,
    py: CEIL_HEIGHT + WALL_HEIGHT / 2,
    shape: {
      width: WALL_WIDTH,
      height: WALL_HEIGHT,
    },
    color: '#654321',
  },
  {
    id: 'platform_1',
    px: WORLD_WIDTH - WALL_WIDTH - PLATFORM_WIDTH / 2,
    py:
      WORLD_HEIGHT - GROUND_HEIGHT - SCREEN_HEIGHT * 0.5 - PLATFORM_HEIGHT / 2,
    shape: { width: PLATFORM_WIDTH, height: PLATFORM_HEIGHT },
    color: '#654321',
  },
  {
    id: 'platform_2',
    px: WALL_WIDTH + PLATFORM_WIDTH / 2,
    py: WORLD_HEIGHT - GROUND_HEIGHT - SCREEN_HEIGHT * 0.8 - PLATFORM_HEIGHT,
    shape: { width: PLATFORM_WIDTH, height: PLATFORM_HEIGHT * 2 },
    color: '#654321',
  },
];
