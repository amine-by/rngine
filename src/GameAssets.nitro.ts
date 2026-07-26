import type { HybridObject } from 'react-native-nitro-modules';
import { NitroModules } from 'react-native-nitro-modules';

interface GameAssets extends HybridObject<{
  ios: 'swift';
  android: 'kotlin';
}> {
  isAssetRegistered(id: number): boolean;
  registerSvg(id: number, uri: string): Promise<void>;
  registerLottie(id: number, json: string): Promise<void>;
}

export const gameAssets =
  NitroModules.createHybridObject<GameAssets>('GameAssets');
