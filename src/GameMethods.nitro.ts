import type { HybridObject } from 'react-native-nitro-modules';

export interface GameMethods extends HybridObject<{
  ios: 'c++';
  android: 'c++';
}> {
  setV(id: string, xv: number, yv: number): void;
}
