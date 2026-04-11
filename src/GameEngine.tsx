import { getHostComponent } from 'react-native-nitro-modules';
import GameEngineConfig from '../nitrogen/generated/shared/json/GameEngineConfig.json';
import type { GameEngineMethods, GameEngineProps } from './GameEngine.nitro';

export default getHostComponent<GameEngineProps, GameEngineMethods>(
  'GameEngine',
  () => GameEngineConfig
);
