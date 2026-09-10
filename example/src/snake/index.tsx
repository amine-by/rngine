import { useCallback, useEffect, useRef, useState } from 'react';
import {
  StyleSheet,
  Text,
  TouchableOpacity,
  useWindowDimensions,
  View,
} from 'react-native';
import {
  SafeAreaProvider,
  useSafeAreaInsets,
  type EdgeInsets,
} from 'react-native-safe-area-context';
import {
  configure,
  GameEngine,
  update,
  pause,
  resume,
  spawn,
  type EntityUpdate,
} from 'rngine';
import { useAssets } from '../AssetsContext';

function Snake() {
  return (
    <SafeAreaProvider>
      <SnakeContent />
    </SafeAreaProvider>
  );
}

type Direction = 'UP' | 'DOWN' | 'LEFT' | 'RIGHT';
type GameState = 'IDLE' | 'PLAYING' | 'PAUSED' | 'OVER';

const CELL = 40;
const HALF_CELL = 20;
const COLS = 20;
const ROWS = 20;

function SnakeContent() {
  const { width, height } = useWindowDimensions();
  const isLandscape = width > height;
  const safeAreaInsets = useSafeAreaInsets();
  const styles = getStyles({ ...safeAreaInsets, isLandscape });
  const directionRef = useRef<Direction>('LEFT');
  const { getAssets } = useAssets();

  const {
    food,
    head_up,
    head_left,
    body_up_left,
    body_horizontal,
    body_vertical,
    tail_up,
    tail_right,
  } = getAssets('Snake') ?? {};

  const [gameState, setGameState] = useState<GameState>('IDLE');

  const start = useCallback(() => {
    configure({
      world: { tickRate: 8 },
      screen: { width: CELL * COLS, height: CELL * ROWS, color: '#1a1a1a' },
      entities: [
        {
          id: 'food',
          px: Math.floor(Math.random() * COLS) * CELL + HALF_CELL,
          py: Math.floor(Math.random() * ROWS) * CELL + HALF_CELL,
          shape: { width: CELL, height: CELL },
          asset: food,
          isSensor: true,
        },
        {
          id: 'snake_head',
          px: 10 * CELL + HALF_CELL,
          py: 10 * CELL + HALF_CELL,
          shape: { width: CELL, height: CELL },
          asset: head_left,
          isSensor: true,
        },
        {
          id: 'snake_body_001',
          px: 11 * CELL + HALF_CELL,
          py: 10 * CELL + HALF_CELL,
          shape: { width: CELL, height: CELL },
          asset: body_horizontal,
          isSensor: true,
        },
        {
          id: 'snake_body_002',
          px: 12 * CELL + HALF_CELL,
          py: 10 * CELL + HALF_CELL,
          shape: { width: CELL, height: CELL },
          asset: tail_right,
          isSensor: true,
        },
      ],
      systems: [
        {
          entities: ['food', 'snake_head', 'snake_body'],
          onTick: (entities) => {
            const updates: (Omit<EntityUpdate, 'px' | 'py'> & {
              px: number;
              py: number;
            })[] = [
              {
                id: entities[1]!.id,
                px: entities[1]!.px,
                py: entities[1]!.py,
              },
            ];

            switch (directionRef.current) {
              case 'UP':
                updates[0]!.py -= CELL;
                break;
              case 'DOWN':
                updates[0]!.py += CELL;
                break;
              case 'LEFT':
                updates[0]!.px -= CELL;
                break;
              case 'RIGHT':
                updates[0]!.px += CELL;
                break;
            }

            if (
              updates[0]!.px < 0 ||
              updates[0]!.py < 0 ||
              updates[0]!.px + CELL > CELL * COLS + HALF_CELL ||
              updates[0]!.py + CELL > CELL * ROWS + HALF_CELL
            ) {
              pause();
              setGameState('OVER');
              return;
            }

            for (let i = 2; i < entities.length; i++) {
              if (
                updates[0]!.px === entities[i - 1]!.px &&
                updates[0]!.py === entities[i - 1]!.py
              ) {
                pause();
                setGameState('OVER');
                return;
              }

              updates.push({
                id: entities[i]!.id,
                px: entities[i - 1]!.px,
                py: entities[i - 1]!.py,
              });
            }

            if (
              entities[1]!.px === entities[0]!.px &&
              entities[1]!.py === entities[0]!.py
            ) {
              updates.push({
                id: 'food',
                px: Math.floor(Math.random() * COLS) * CELL + HALF_CELL,
                py: Math.floor(Math.random() * ROWS) * CELL + HALF_CELL,
              });

              spawn({
                id: `snake_body_${
                  entities.length - 1 < 10
                    ? '00'
                    : entities.length - 1 < 100
                      ? '0'
                      : ''
                }${entities.length - 1}`,
                px: entities[entities.length - 1]!.px,
                py: entities[entities.length - 1]!.py,
                shape: { width: CELL, height: CELL },
                isSensor: true,
              });
            }
            update(updates);
          },
        },
        {
          entities: ['snake_head', 'snake_body'],
          onTick: (entities) => {
            const updates: EntityUpdate[] = [];
            const HEAD_ASSET_MAP = {
              UP: { asset: head_up, flipH: false, flipV: false },
              DOWN: { asset: head_up, flipH: false, flipV: true },
              LEFT: { asset: head_left, flipH: false, flipV: false },
              RIGHT: { asset: head_left, flipH: true, flipV: false },
            };

            updates.push({
              id: entities[0]!.id,
              ...HEAD_ASSET_MAP[directionRef.current],
            });

            const BODY_ASSET_MAP = {
              LEFT_UP: { asset: body_up_left, flipH: false, flipV: false },
              DOWN_RIGHT: {
                asset: body_up_left,
                flipH: true,
                flipV: true,
              },
              DOWN_LEFT: {
                asset: body_up_left,
                flipH: false,
                flipV: true,
              },
              RIGHT_UP: { asset: body_up_left, flipH: true, flipV: false },
              HORIZONTAL: {
                asset: body_horizontal,
                flipH: false,
                flipV: false,
              },
              VERTICAL: {
                asset: body_vertical,
                flipH: false,
                flipV: false,
              },
            };

            let bodyKey: keyof typeof BODY_ASSET_MAP;

            for (let i = 1; i < entities.length - 1; i++) {
              const previousPx = entities[i - 1]!.px;
              const previousPy = entities[i - 1]!.py;
              const currentPx = entities[i]!.px;
              const currentPy = entities[i]!.py;
              const nextPx = entities[i + 1]!.px;
              const nextPy = entities[i + 1]!.py;

              if (previousPx === currentPx && currentPx === nextPx) {
                bodyKey = 'VERTICAL';
              } else if (previousPy === currentPy && currentPy === nextPy) {
                bodyKey = 'HORIZONTAL';
              } else {
                const dirTowardHead =
                  previousPx > currentPx
                    ? 'RIGHT'
                    : previousPx < currentPx
                      ? 'LEFT'
                      : previousPy > currentPy
                        ? 'DOWN'
                        : 'UP';

                const dirTowardTail =
                  nextPx > currentPx
                    ? 'RIGHT'
                    : nextPx < currentPx
                      ? 'LEFT'
                      : nextPy > currentPy
                        ? 'DOWN'
                        : 'UP';

                const pair = [dirTowardHead, dirTowardTail].sort().join('_') as
                  | 'LEFT_UP'
                  | 'DOWN_RIGHT'
                  | 'DOWN_LEFT'
                  | 'RIGHT_UP';

                bodyKey = pair;
              }

              updates.push({
                id: entities[i]!.id,
                ...BODY_ASSET_MAP[bodyKey],
              });
            }

            const TAIL_ASSET_MAP = {
              UP: { asset: tail_up, flipH: false, flipV: false },
              DOWN: { asset: tail_up, flipH: false, flipV: true },
              LEFT: { asset: tail_right, flipH: true, flipV: false },
              RIGHT: { asset: tail_right, flipH: false, flipV: false },
            };

            let tailKey: keyof typeof TAIL_ASSET_MAP;

            if (
              entities[entities.length - 1]!.px ===
              entities[entities.length - 2]!.px
            ) {
              if (
                entities[entities.length - 1]!.py >
                entities[entities.length - 2]!.py
              ) {
                tailKey = 'DOWN';
              } else {
                tailKey = 'UP';
              }
            } else {
              if (
                entities[entities.length - 1]!.px >
                entities[entities.length - 2]!.px
              ) {
                tailKey = 'RIGHT';
              } else {
                tailKey = 'LEFT';
              }
            }
            updates.push({
              id: entities[entities.length - 1]!.id,
              ...TAIL_ASSET_MAP[tailKey],
            });

            update(updates);

            return;
          },
        },
      ],
    });
  }, [
    food,
    head_up,
    head_left,
    body_up_left,
    body_horizontal,
    body_vertical,
    tail_up,
    tail_right,
  ]);

  const togglePause = () => {
    if (gameState === 'OVER') {
      start();
      setGameState('IDLE');
      directionRef.current = 'LEFT';
    } else if (gameState === 'PLAYING') {
      pause();
      setGameState('PAUSED');
    } else {
      resume();
      setGameState('PLAYING');
    }
  };

  const changeDirection = (newDirection: Direction) => {
    if (gameState === 'OVER') {
      return;
    }

    if (gameState === 'IDLE') {
      resume();
      setGameState('PLAYING');
    }

    if (
      (newDirection === 'UP' && directionRef.current === 'DOWN') ||
      (newDirection === 'DOWN' && directionRef.current === 'UP') ||
      (newDirection === 'LEFT' && directionRef.current === 'RIGHT') ||
      (newDirection === 'RIGHT' && directionRef.current === 'LEFT')
    ) {
      return;
    }
    directionRef.current = newDirection;
  };

  useEffect(() => {
    start();
  }, [start]);

  return (
    <View style={styles.screen}>
      <View style={styles.container}>
        <GameEngine style={styles.game} />
        <View style={styles.controls}>
          <TouchableOpacity style={styles.pauseBtn} onPress={togglePause}>
            <Text style={styles.btnText}>
              {gameState === 'OVER'
                ? '⟳'
                : gameState === 'PLAYING'
                  ? '❚❚'
                  : '▶'}
            </Text>
          </TouchableOpacity>
          <View style={styles.dpad}>
            <TouchableOpacity
              style={styles.btn}
              onPress={() => changeDirection('UP')}
            >
              <Text style={styles.btnText}>▲</Text>
            </TouchableOpacity>
            <View style={styles.dpadRow}>
              <TouchableOpacity
                style={styles.btn}
                onPress={() => changeDirection('LEFT')}
              >
                <Text style={styles.btnText}>◀</Text>
              </TouchableOpacity>
              <TouchableOpacity
                style={styles.btn}
                onPress={() => changeDirection('DOWN')}
              >
                <Text style={styles.btnText}>▼</Text>
              </TouchableOpacity>
              <TouchableOpacity
                style={styles.btn}
                onPress={() => changeDirection('RIGHT')}
              >
                <Text style={styles.btnText}>▶</Text>
              </TouchableOpacity>
            </View>
          </View>
        </View>
      </View>
    </View>
  );
}

const getStyles = ({
  top,
  right,
  bottom,
  left,
  isLandscape,
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
      flexDirection: isLandscape ? 'row' : 'column',
    },
    game: {
      flex: 1,
    },
    controls: {
      flexDirection: isLandscape ? 'column' : 'row-reverse',
      justifyContent: 'space-between',
      marginHorizontal: 16,
      marginVertical: 32,
    },
    pauseBtn: {
      backgroundColor: 'blue',
      width: 60,
      height: 60,
      borderRadius: 8,
      alignItems: 'center',
      justifyContent: 'center',
    },
    dpad: {
      alignSelf: 'center',
      alignItems: 'center',
      gap: 8,
    },
    dpadRow: {
      flexDirection: 'row',
      alignItems: 'center',
      gap: 8,
    },
    btn: {
      backgroundColor: 'blue',
      width: 60,
      height: 60,
      borderRadius: 8,
      alignItems: 'center',
      justifyContent: 'center',
    },
    btnText: {
      color: '#fff',
      fontSize: 20,
    },
  });
};

export default Snake;
