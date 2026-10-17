import { getAssets } from '../../assets/assetsManager';
import { CELL, COLS, HALF_CELL, ROWS } from './consts';

const { Food, Head, Body, Tail } = getAssets('Snake') ?? {};

export const entities = [
  {
    id: 'food',
    px: Math.floor(Math.random() * COLS) * CELL + HALF_CELL,
    py: Math.floor(Math.random() * ROWS) * CELL + HALF_CELL,
    shape: { width: CELL, height: CELL },
    asset: Food,
    isSensor: true,
  },
  {
    id: 'snake_head',
    px: 10 * CELL + HALF_CELL,
    py: 10 * CELL + HALF_CELL,
    shape: { width: CELL, height: CELL },
    asset: Head,
    isSensor: true,
  },
  {
    id: 'snake_body_001',
    px: 11 * CELL + HALF_CELL,
    py: 10 * CELL + HALF_CELL,
    shape: { width: CELL, height: CELL },
    asset: Body,
    isSensor: true,
  },
  {
    id: 'snake_body_002',
    px: 12 * CELL + HALF_CELL,
    py: 10 * CELL + HALF_CELL,
    shape: { width: CELL, height: CELL },
    asset: Tail,
    isSensor: true,
  },
];
