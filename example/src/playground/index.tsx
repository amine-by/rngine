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

const EPSILON = 1e-10;

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

  const { Background_Test_Svg, Idle, Run, Jump, Fall } =
    getAssets('Playground') ?? {};

  useEffect(() => {
    configure({
      paused: false,
      world: { tickRate: 60, gy: 2000 },
      screen: {
        width: 800,
        height: 800,
        asset: Background_Test_Svg,
      },
      entities: [
        {
          id: 'player',
          px: 120,
          py: 640,
          shape: { width: 92, height: 116 },
          asset: Fall,
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
        {
          id: 'platform_1',
          px: 280,
          py: 520,
          shape: { width: 560, height: 80 },
          color: '#654321',
        },
        {
          id: 'platform_2',
          px: 520,
          py: 200,
          shape: { width: 560, height: 80 },
          color: '#654321',
        },
      ],
      systems: [
        {
          entities: ['player'],
          onTick: (entities) => {
            if (
              entities[0]?.vx === 0 &&
              Math.abs(entities[0]?.vy ?? 0) < EPSILON &&
              entities[0]?.asset !== Idle
            ) {
              update({
                id: 'player',
                asset: Idle,
                shape: { width: 84, height: 116 },
              });
            } else if (
              entities[0]?.vx !== 0 &&
              Math.abs(entities[0]?.vy ?? 0) < EPSILON &&
              entities[0]?.asset !== Run
            ) {
              update({
                id: 'player',
                asset: Run,
                shape: { width: 92, height: 120 },
              });
            } else if (
              typeof entities[0]?.vy !== 'undefined' &&
              !isNaN(entities[0]?.vy) &&
              entities[0]?.vy < -EPSILON &&
              entities[0]?.asset !== Jump
            ) {
              update({
                id: 'player',
                asset: Jump,
                shape: { width: 84, height: 124 },
              });
            } else if (
              typeof entities[0]?.vy !== 'undefined' &&
              !isNaN(entities[0]?.vy) &&
              entities[0]?.vy > EPSILON &&
              entities[0]?.asset !== Fall
            ) {
              update({
                id: 'player',
                asset: Fall,
                shape: { width: 92, height: 116 },
              });
            }
          },
        },
      ],
    });
  }, [Background_Test_Svg, Fall, Idle, Run, Jump]);

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
    update({ id: 'player', vy: -1200 });
  };

  const moveLeft = () => {
    if (isPaused) return;
    update({
      id: 'player',
      vx: -500,
      flipH: true,
    });
  };

  const moveRight = () => {
    if (isPaused) return;
    update({
      id: 'player',
      vx: 500,
      flipH: false,
    });
  };

  const stop = () => {
    update({
      id: 'player',
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
