import type {
  Screen as NativeScreen,
  Entity as NativeEntity,
  EntityUpdate as NativeEntityUpdate,
  Config as NativeConfig,
} from './nativeTypes';

export type Asset = number | object;

export type Screen = Omit<NativeScreen, 'asset'> & { asset?: Asset };
export type Entity = Omit<NativeEntity, 'asset'> & { asset?: Asset };
export type EntityUpdate = Omit<NativeEntityUpdate, 'asset'> & {
  asset?: Asset;
};
export type Config = Omit<NativeConfig, 'screen' | 'entities'> & {
  screen: Screen;
  entities?: Entity[];
};
