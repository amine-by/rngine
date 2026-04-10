import type {
  HybridView,
  HybridViewMethods,
  HybridViewProps,
} from 'react-native-nitro-modules';

export interface RngineProps extends HybridViewProps {
  color: string;
}
export interface RngineMethods extends HybridViewMethods {}

export type Rngine = HybridView<
  RngineProps,
  RngineMethods
>;
