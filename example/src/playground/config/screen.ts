import type { Screen } from 'rngine';
import { getAssets } from '../../assets/assetsManager';
import { SCREEN_HEIGHT, SCREEN_WIDTH, WORLD_HEIGHT } from './consts';

const { Background_Tile } = getAssets('Playground') ?? {};

export const screen: Screen = {
  px: SCREEN_WIDTH / 2,
  py: WORLD_HEIGHT - SCREEN_HEIGHT / 2,
  width: SCREEN_WIDTH,
  height: SCREEN_HEIGHT,
  asset: Background_Tile,
  objectFit: 'none',
  repeat: 'repeat',
  color: '#fff',
};
