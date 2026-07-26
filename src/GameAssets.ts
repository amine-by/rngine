import { Image } from 'react-native';
import { gameAssets } from './GameAssets.nitro';

const assetCache = new WeakMap<object, number>();
let nextId = -1;

const loadAsset = async (asset: unknown) => {
  switch (typeof asset) {
    case 'object':
      if (asset === null) {
        throw new Error('loadAssets: asset cannot be null');
      }
      if (assetCache.has(asset)) {
        return assetCache.get(asset)!;
      }
      const assetId = nextId--;
      assetCache.set(asset, assetId);
      await gameAssets.registerLottie(assetId, JSON.stringify(asset));
      return assetId;

    case 'number':
      if (gameAssets.isAssetRegistered(asset)) {
        return asset;
      }
      const assetUri = Image.resolveAssetSource(asset)?.uri;
      if (!assetUri) {
        throw new Error(`loadAssets: could not resolve asset URI for ${asset}`);
      }
      await gameAssets.registerSvg(asset, assetUri);
      return asset;
  }

  throw new Error(`loadAssets: unsupported asset type "${typeof asset}"`);
};

export const loadAssets = async <T extends Record<string, unknown>>(
  assets: T
): Promise<{ [K in keyof T]: number }> => {
  const keys = Object.keys(assets) as (keyof T)[];
  const result = {} as { [K in keyof T]: number };

  for (const key of keys) {
    result[key] = await loadAsset(assets[key]);
  }

  return result;
};
