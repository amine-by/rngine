import type {
  HybridView,
  HybridViewMethods,
  HybridViewProps,
} from 'react-native-nitro-modules';

export interface GameEngineProps extends HybridViewProps {}
export interface GameEngineMethods extends HybridViewMethods {}

export type GameEngine = HybridView<GameEngineProps, GameEngineMethods>;
