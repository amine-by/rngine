import { NitroModules, type HybridObject } from 'react-native-nitro-modules';

interface GameLoop extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  pause(): void;
  resume(): void;
}

export default () => NitroModules.createHybridObject<GameLoop>('GameLoop');
