import { loadAssets } from 'rngine';
import ASSETS from '.';

export type GameType = keyof typeof ASSETS;

type AssetsType = {
  [K in GameType]?: Record<keyof (typeof ASSETS)[K], number>;
};

let assetsCache: AssetsType = {};

export async function loadAssetsFor(game: GameType): Promise<void> {
  const loadedAssets = await loadAssets(ASSETS[game]);
  assetsCache = { ...assetsCache, [game]: loadedAssets };
}

export function getAssets<T extends GameType>(game: T): AssetsType[T] {
  return assetsCache[game];
}
