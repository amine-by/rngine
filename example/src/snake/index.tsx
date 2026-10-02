import { useCallback, useEffect, useSyncExternalStore } from 'react';
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
import { configure, GameEngine, pause, resume } from 'rngine';
import {
  setGameState,
  snakeState,
  subscribeToGameState,
  type Direction,
} from './config/state';
import { screen } from './config/screen';
import { systems } from './config/systems';
import { entities } from './config/entities';

function Snake() {
  return (
    <SafeAreaProvider>
      <SnakeContent />
    </SafeAreaProvider>
  );
}

function SnakeContent() {
  const { width, height } = useWindowDimensions();
  const isLandscape = width > height;
  const safeAreaInsets = useSafeAreaInsets();
  const styles = getStyles({ ...safeAreaInsets, isLandscape });

  const gameState = useSyncExternalStore(
    subscribeToGameState,
    () => snakeState.gameState
  );

  const start = useCallback(() => {
    configure({
      world: { tickRate: 8 },
      screen,
      entities,
      systems,
    });
  }, []);

  const togglePause = () => {
    if (gameState === 'OVER') {
      start();
      setGameState('IDLE');
      snakeState.direction = 'LEFT';
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
      (newDirection === 'UP' && snakeState.direction === 'DOWN') ||
      (newDirection === 'DOWN' && snakeState.direction === 'UP') ||
      (newDirection === 'LEFT' && snakeState.direction === 'RIGHT') ||
      (newDirection === 'RIGHT' && snakeState.direction === 'LEFT')
    ) {
      return;
    }
    snakeState.direction = newDirection;
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
