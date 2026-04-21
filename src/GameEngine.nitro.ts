import type {
  HybridView,
  HybridViewMethods,
  HybridViewProps,
} from 'react-native-nitro-modules';
import type { Entity } from './types';

export interface GameEngineProps extends HybridViewProps {
  isPaused: boolean;
  initialEntities: Entity[];
}
export interface GameEngineMethods extends HybridViewMethods {}

export type GameEngine = HybridView<GameEngineProps, GameEngineMethods>;
