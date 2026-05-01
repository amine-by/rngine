import { getHostComponent } from 'react-native-nitro-modules';
const GameEngineConfig = require('../nitrogen/generated/shared/json/GameEngineConfig.json');
import type { GameEngineMethods, GameEngineProps } from './GameEngine.nitro';

/**
 * A game engine view component that renders the game world.
 * Size it using the style prop like any other React Native view.
 *
 * @example
 * ```tsx
 * <GameEngine style={{ flex: 1 }} />
 * ```
 */
export default getHostComponent<GameEngineProps, GameEngineMethods>(
  'GameEngine',
  () => GameEngineConfig
);
