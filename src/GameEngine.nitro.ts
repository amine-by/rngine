import type {
  HybridView,
  HybridViewMethods,
  HybridViewProps,
} from 'react-native-nitro-modules';

type Entity = {
  id: string;
  x: number;
  y: number;
  width: number;
  height: number;
  xv: number;
  yv: number;
};

export interface GameEngineProps extends HybridViewProps {
  isPaused: boolean;
  initialEntities: Entity[];
}
export interface GameEngineMethods extends HybridViewMethods {}

export type GameEngine = HybridView<GameEngineProps, GameEngineMethods>;
