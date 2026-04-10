import { getHostComponent } from 'react-native-nitro-modules';
const RngineConfig = require('../nitrogen/generated/shared/json/RngineConfig.json');
import type {
  RngineMethods,
  RngineProps,
} from './Rngine.nitro';

export const RngineView = getHostComponent<
  RngineProps,
  RngineMethods
>('Rngine', () => RngineConfig);
