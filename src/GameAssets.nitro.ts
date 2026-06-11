import type { HybridObject } from 'react-native-nitro-modules';
import { NitroModules } from 'react-native-nitro-modules';

interface GameAssets extends HybridObject<{
  ios: 'swift';
  android: 'kotlin';
}> {
  isAssetRegistered(id: number): boolean;
  registerSvg(id: number, uri: string): void;
  registerLottie(id: number, json: string): void;
}

export const gameAssets =
  NitroModules.createHybridObject<GameAssets>('GameAssets');
