import type {
  HybridView,
  HybridViewMethods,
  HybridViewProps,
} from 'react-native-nitro-modules';

type Entity = {
  id: string;
  px: number;
  py: number;
  width: number;
  height: number;
  vx: number;
  vy: number;
};

export interface GameEngineProps extends HybridViewProps {
  isPaused: boolean;
  initialEntities: Entity[];
}
export interface GameEngineMethods extends HybridViewMethods {}

export type GameEngine = HybridView<GameEngineProps, GameEngineMethods>;
