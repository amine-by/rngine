import type { HybridObject } from 'react-native-nitro-modules';

export interface GameMethods extends HybridObject<{
  ios: 'c++';
  android: 'c++';
}> {
  setP(id: string, px: number, py: number): void;
  setV(id: string, vx: number, vy: number): void;
}
