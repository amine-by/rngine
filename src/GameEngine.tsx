import { getHostComponent } from 'react-native-nitro-modules';
const GameEngineConfig = require('../nitrogen/generated/shared/json/GameEngineConfig.json');
import type { GameEngineMethods, GameEngineProps } from './GameEngine.nitro';

export default getHostComponent<GameEngineProps, GameEngineMethods>(
  'GameEngine',
  () => GameEngineConfig
);
