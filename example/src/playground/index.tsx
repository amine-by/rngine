import { useEffect, useState } from 'react';
import { StyleSheet, useWindowDimensions, View } from 'react-native';
import { GameEngine, configure, pause, resume, update } from 'rngine';
import { ControlButton } from './components/ControlButton';
import { useAssets } from '../AssetsContext';
import {
  GestureHandlerRootView,
  useLongPressGesture,
  useTapGesture,
} from 'react-native-gesture-handler';
import {
  SafeAreaProvider,
  useSafeAreaInsets,
  type EdgeInsets,
} from 'react-native-safe-area-context';

function Playground() {
  return (
    <SafeAreaProvider>
      <PlaygroundContent />
    </SafeAreaProvider>
  );
}

function PlaygroundContent() {
  const [isPaused, setIsPaused] = useState(false);
  const { getAssets } = useAssets();

  const { width, height } = useWindowDimensions();
  const isLandscape = width > height;
  const safeAreaInsets = useSafeAreaInsets();
  const styles = getStyles({ ...safeAreaInsets, isLandscape });

  const { background_test_svg, idle, run } = getAssets('Playground') ?? {};

  useEffect(() => {
    configure({
      paused: false,
      world: { tickRate: 60, gy: 1000 },
      screen: {
        width: 800,
        height: 800,
        asset: background_test_svg,
      },
      entities: [
        {
          id: 'player',
          px: 300,
          py: 100,
          shape: { width: 84, height: 116 },
          asset: idle,
          speed: 1,
          mass: 5,
        },
        {
          id: 'ground',
          px: 400,
          py: 760,
          shape: { width: 800, height: 80 },
          color: '#654321',
        },
      ],
    });
  }, [background_test_svg, idle]);

  const onTogglePause = () => {
    setIsPaused((prev) => {
      if (prev) {
        resume();
      } else {
        pause();
      }
      return !prev;
    });
  };

  const jump = () => {
    if (isPaused) return;
    update({ id: 'player', vy: -600 });
  };

  const moveLeft = () => {
    if (isPaused) return;
    update({
      id: 'player',
      asset: run,
      progress: 0,
      shape: { width: 92, height: 120 },
      vx: -500,
      flipH: true,
    });
  };

  const moveRight = () => {
    if (isPaused) return;
    update({
      id: 'player',
      asset: run,
      progress: 0,
      shape: { width: 92, height: 120 },
      vx: 500,
      flipH: false,
    });
  };

  const stop = () => {
    update({
      id: 'player',
      asset: idle,
      progress: 0,
      shape: { width: 84, height: 116 },
      vx: 0,
    });
  };

  const moveLeftGesture = useLongPressGesture({
    onBegin: moveLeft,
    onFinalize: stop,
  });

  const moveRightGesture = useLongPressGesture({
    onBegin: moveRight,
    onFinalize: stop,
  });

  const onTogglePauseGesture = useTapGesture({
    onActivate: onTogglePause,
  });

  const jumpGesture = useTapGesture({
    onActivate: jump,
  });

  return (
    <GestureHandlerRootView style={styles.screen}>
      <View style={styles.container}>
        <GameEngine style={styles.gameEngine} />
        <ControlButton
          style={styles.togglePauseButtonContainer}
          gesture={onTogglePauseGesture}
        >
          {isPaused ? 'Resume' : 'Pause'}
        </ControlButton>
        <ControlButton style={styles.jumpContainer} gesture={jumpGesture}>
          Jump
        </ControlButton>
        <View style={styles.moveButtonsContainer}>
          <ControlButton gesture={moveLeftGesture}>Left</ControlButton>
          <ControlButton gesture={moveRightGesture}>Right</ControlButton>
        </View>
      </View>
    </GestureHandlerRootView>
  );
}

const getStyles = ({
  top,
  right,
  bottom,
  left,
}: EdgeInsets & { isLandscape: boolean }) => {
  return StyleSheet.create({
    screen: {
      flex: 1,
      backgroundColor: 'black',
    },
    container: {
      position: 'absolute',
      top,
      right,
      bottom,
      left,
    },
    togglePauseButtonContainer: {
      position: 'absolute',
      top: 32,
      right: 16,
    },
    jumpContainer: {
      position: 'absolute',
      bottom: 32,
      left: 16,
    },
    moveButtonsContainer: {
      position: 'absolute',
      flexDirection: 'row',
      bottom: 32,
      right: 16,
      gap: 8,
      alignItems: 'flex-end',
    },
    gameEngine: { flex: 1 },
  });
};

export default Playground;
