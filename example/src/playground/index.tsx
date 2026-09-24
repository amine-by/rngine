import { useEffect, useRef, useState } from 'react';
import { StyleSheet, useWindowDimensions, View } from 'react-native';
import {
  GameEngine,
  configure,
  pause,
  resume,
  updateEntity,
  updateScreen,
} from 'rngine';
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
import type { EntityUpdate, ScreenUpdate } from '../../../src/nativeTypes';

const EPSILON = 1e-10;
const SCREEN_WIDTH = 800;
const SCREEN_HEIGHT = 800;
const WORLD_WIDTH = SCREEN_WIDTH * 2;
const WORLD_HEIGHT = SCREEN_HEIGHT * 2;
const GROUND_HEIGHT = SCREEN_HEIGHT / 10;
const CEIL_HEIGHT = SCREEN_HEIGHT / 10;
const WALL_WIDTH = SCREEN_WIDTH / 10;
const WALL_HEIGHT = WORLD_HEIGHT - GROUND_HEIGHT - CEIL_HEIGHT;
const PLATFORM_WIDTH = WORLD_HEIGHT - WALL_WIDTH * 2 - SCREEN_WIDTH * 0.4;
const PLATFORM_HEIGHT = SCREEN_HEIGHT / 10;

function Playground() {
  return (
    <SafeAreaProvider>
      <PlaygroundContent />
    </SafeAreaProvider>
  );
}

function PlaygroundContent() {
  const doubleJumpRef = useRef<boolean>(false);

  const [isPaused, setIsPaused] = useState(false);
  const { getAssets } = useAssets();

  const { width, height } = useWindowDimensions();
  const isLandscape = width > height;
  const safeAreaInsets = useSafeAreaInsets();
  const styles = getStyles({ ...safeAreaInsets, isLandscape });

  const { Background_Test_Svg, Idle, Run, Jump, Double_Jump, Fall } =
    getAssets('Playground') ?? {};

  useEffect(() => {
    configure({
      paused: false,
      world: { tickRate: 60, gy: 2000 },
      screen: {
        px: SCREEN_WIDTH / 2,
        py: WORLD_HEIGHT - SCREEN_HEIGHT / 2,
        width: SCREEN_WIDTH,
        height: SCREEN_HEIGHT,
        asset: Background_Test_Svg,
        color: '#fff',
      },
      entities: [
        {
          id: 'player',
          px: SCREEN_WIDTH / 4,
          py: WORLD_HEIGHT - GROUND_HEIGHT - 116 / 2,
          shape: { width: 84, height: 116 },
          asset: Idle,
          speed: 1,
          loop: true,
          mass: 5,
        },
        {
          id: 'ground',
          px: WORLD_WIDTH / 2,
          py: WORLD_HEIGHT - GROUND_HEIGHT / 2,
          shape: { width: WORLD_WIDTH, height: GROUND_HEIGHT },
          color: '#654321',
        },
        {
          id: 'ceil',
          px: WORLD_WIDTH / 2,
          py: CEIL_HEIGHT / 2,
          shape: { width: WORLD_WIDTH, height: CEIL_HEIGHT },
          color: '#654321',
        },
        {
          id: 'wall_left',
          px: WALL_WIDTH / 2,
          py: CEIL_HEIGHT + WALL_HEIGHT / 2,
          shape: {
            width: WALL_WIDTH,
            height: WALL_HEIGHT,
          },
          color: '#654321',
        },
        {
          id: 'wall_right',
          px: WORLD_WIDTH - WALL_WIDTH / 2,
          py: CEIL_HEIGHT + WALL_HEIGHT / 2,
          shape: {
            width: WALL_WIDTH,
            height: WALL_HEIGHT,
          },
          color: '#654321',
        },
        {
          id: 'platform_1',
          px: WALL_WIDTH + PLATFORM_WIDTH / 2,
          py:
            WORLD_HEIGHT -
            GROUND_HEIGHT -
            SCREEN_HEIGHT * 0.2 -
            PLATFORM_HEIGHT / 2,
          shape: { width: PLATFORM_WIDTH, height: PLATFORM_HEIGHT },
          color: '#654321',
        },
        {
          id: 'platform_2',
          px: WORLD_WIDTH - WALL_WIDTH - PLATFORM_WIDTH / 2,
          py:
            WORLD_HEIGHT -
            GROUND_HEIGHT -
            SCREEN_HEIGHT * 0.5 -
            PLATFORM_HEIGHT / 2,
          shape: { width: PLATFORM_WIDTH, height: PLATFORM_HEIGHT },
          color: '#654321',
        },
        {
          id: 'platform_3',
          px: WALL_WIDTH + PLATFORM_WIDTH / 2,
          py:
            WORLD_HEIGHT -
            GROUND_HEIGHT -
            SCREEN_HEIGHT * 0.8 -
            PLATFORM_HEIGHT,
          shape: { width: PLATFORM_WIDTH, height: PLATFORM_HEIGHT * 2 },
          color: '#654321',
        },
      ],
      systems: [
        {
          entities: ['player'],
          onTick: ({ entities, screen }) => {
            const player = entities[0];

            if (!player) {
              return;
            }

            let entityUpdate: EntityUpdate = {
              id: 'player',
              loop: true,
              progress: 0,
            };

            let screenUpdate: ScreenUpdate = {};

            if (
              player.vx === 0 &&
              Math.abs(player?.vy ?? 0) < EPSILON &&
              player?.asset !== Idle
            ) {
              entityUpdate.asset = Idle;
              entityUpdate.shape = { width: 84, height: 116 };
            } else if (
              player.vx !== 0 &&
              Math.abs(player?.vy ?? 0) < EPSILON &&
              player?.asset !== Run
            ) {
              entityUpdate.asset = Run;
              entityUpdate.shape = { width: 92, height: 120 };
            } else if (
              typeof player?.vy !== 'undefined' &&
              !isNaN(player?.vy) &&
              player?.vy < -EPSILON &&
              player?.asset !== Jump &&
              player?.asset !== Double_Jump
            ) {
              entityUpdate.asset = Jump;
              entityUpdate.shape = { width: 84, height: 124 };
            } else if (
              typeof player?.vy !== 'undefined' &&
              !isNaN(player?.vy) &&
              player?.vy > EPSILON &&
              player?.asset !== Fall &&
              player?.asset !== Double_Jump
            ) {
              entityUpdate.asset = Fall;
              entityUpdate.shape = { width: 92, height: 116 };
            } else if (
              player?.asset === Double_Jump &&
              player?.progress === 1
            ) {
              entityUpdate.asset = Jump;
              entityUpdate.shape = { width: 84, height: 124 };
            }

            if (player.px > SCREEN_WIDTH && screen.px < SCREEN_WIDTH) {
              screenUpdate.px = SCREEN_WIDTH * 1.5;
            } else if (player.px < SCREEN_WIDTH && screen.px > SCREEN_WIDTH) {
              screenUpdate.px = SCREEN_WIDTH * 0.5;
            }

            if (player.py > SCREEN_HEIGHT && screen.py < SCREEN_HEIGHT) {
              screenUpdate.py = SCREEN_HEIGHT * 1.5;
            } else if (player.py < SCREEN_HEIGHT && screen.py > SCREEN_HEIGHT) {
              screenUpdate.py = SCREEN_HEIGHT * 0.5;
            }

            if (
              typeof entityUpdate.asset !== 'undefined' ||
              typeof entityUpdate.shape !== 'undefined'
            ) {
              updateEntity(entityUpdate);
            }

            if (
              typeof screenUpdate.px !== 'undefined' ||
              typeof screenUpdate.py !== 'undefined'
            ) {
              updateScreen(screenUpdate);
            }
          },
        },
      ],
    });
  }, [Background_Test_Svg, Fall, Idle, Run, Jump, Double_Jump]);

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
    updateEntity({
      id: 'player',
      vy: -1300,
      asset: doubleJumpRef.current ? Double_Jump : undefined,
      loop: !doubleJumpRef.current,
      shape: doubleJumpRef.current ? { width: 104, height: 104 } : undefined,
    });
    doubleJumpRef.current = !doubleJumpRef.current;
  };

  const moveLeft = () => {
    if (isPaused) return;
    updateEntity({
      id: 'player',
      vx: -500,
      flipH: true,
    });
  };

  const moveRight = () => {
    if (isPaused) return;
    updateEntity({
      id: 'player',
      vx: 500,
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
