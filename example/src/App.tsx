import {
  createStaticNavigation,
  type StaticParamList,
} from '@react-navigation/native';
import {
  createNativeStackNavigator,
  type NativeStackNavigationProp,
} from '@react-navigation/native-stack';
import Menu from './menu';
import { lazy } from 'react';
import { AssetsProvider } from './AssetsContext';

const Playground = lazy(() => import('./playground'));
const Snake = lazy(() => import('./snake'));

const RootStack = createNativeStackNavigator({
  initialRouteName: 'Menu',
  screens: {
    Menu: { screen: Menu },
    Playground: {
      screen: Playground,
      options: {
        headerShown: false,
      },
    },
    Snake: {
      screen: Snake,
      options: {
        headerShown: false,
      },
    },
  },
});

const Navigation = createStaticNavigation(RootStack);

export type NavigationProp = NativeStackNavigationProp<
  StaticParamList<typeof RootStack>,
  'Menu'
>;

export default function App() {
  return (
    <AssetsProvider>
      <Navigation />
    </AssetsProvider>
  );
}
