import { useEffect, useState } from 'react';
import { StyleSheet, useWindowDimensions, View } from 'react-native';
import { GameEngine, configure, pause, resume, updateEntity } from 'rngine';
import { ControlButton } from './components/ControlButton';
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
import { getAssets } from '../assets/assetsManager';
import { entities } from './config/entities';
import { systems } from './config/systems';
import { screen } from './config/screen';
import { playerState } from './config/state';

function Playground() {
  return (
    <SafeAreaProvider>
      <PlaygroundContent />
    </SafeAreaProvider>
  );
}

function PlaygroundContent() {
  const [isPaused, setIsPaused] = useState(true);

  const { width, height } = useWindowDimensions();
  const isLandscape = width > height;
  const safeAreaInsets = useSafeAreaInsets();
  const styles = getStyles({ ...safeAreaInsets, isLandscape });

  const { Double_Jump } = getAssets('Playground') ?? {};

  useEffect(() => {
    playerState.availableJumps = 2;

    configure({
      paused: true,
      world: { tickRate: 60, gy: 500 },
      screen,
      entities,
      systems,
    });
  }, []);

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
    if (playerState.availableJumps < 1) return;

    const hasOneRemainingJump = playerState.availableJumps === 1;

    updateEntity({
      id: 'player',
      vy: -250,
      asset: hasOneRemainingJump ? Double_Jump : undefined,
      loop: !hasOneRemainingJump,
      shape: hasOneRemainingJump ? { width: 26, height: 26 } : undefined,
      progress: 0,
    });

    playerState.availableJumps -= 1;
  };

  const moveLeft = () => {
    if (isPaused) return;
    updateEntity({
      id: 'player',
      vx: -100,
      flipH: true,
    });
  };

  const moveRight = () => {
    if (isPaused) return;
    updateEntity({
      id: 'player',
      vx: 100,
      flipH: false,
    });
  };

  const stop = () => {
    updateEntity({
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
