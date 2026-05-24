import { useState } from 'react';
import { StyleSheet, View } from 'react-native';
import {
  GameEngine,
  configure,
  pause,
  resume,
  update,
  spawn,
  despawn,
} from 'rngine';
import { ControlButton } from './components/ControlButton';

const backgroundAsset = require('../assets/background-test.svg');
const asset = require('../assets/test.svg');

configure({
  tickRate: 60,
  screen: {
    width: 800,
    height: 800,
    color: '#fff',
    asset: backgroundAsset,
  },
  entities: [
    {
      id: 'entity_1',
      px: 300,
      py: 300,
      width: 52,
      height: 84,
      color: '#00ff00',
      asset,
    },
  ],
  paused: false,
});

export default function App() {
  const [isPaused, setIsPaused] = useState(false);

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

  const spawn2 = () => {
    if (isPaused) return;
    spawn([
      {
        id: 'entity_2',
        px: 500,
        py: 300,
        width: 52,
        height: 84,
        color: '#ff0000',
        asset,
      },
      {
        id: 'entity_3',
        px: 300,
        py: 500,
        width: 52,
        height: 84,
        color: '#0000ff',
        asset,
      },
    ]);
  };

  const despawnAll = () => {
    if (isPaused) return;
    despawn('entity');
  };

  const move = (vx: number, vy: number) => {
    if (isPaused) return;
    update({ id: 'entity', vx, vy });
  };

  const reposition = () => {
    if (isPaused) return;

    update({ id: 'entity_1', px: 300, py: 300 });
  };
  return (
    <View style={styles.container}>
      <GameEngine style={styles.gameEngine} />
      <View style={styles.systemButtonsContainer}>
        <ControlButton onPress={onTogglePause}>
          {isPaused ? 'Resume' : 'Pause'}
        </ControlButton>
        <ControlButton onPress={reposition}>Repo</ControlButton>
        <ControlButton onPress={spawn2}>Spawn</ControlButton>
        <ControlButton onPress={despawnAll}>Despawn</ControlButton>
      </View>
      <View style={styles.dPadContainer}>
        <ControlButton
          onPress={() => {
            move(-500, 0);
          }}
        >
          Left
        </ControlButton>
        <View style={styles.verticalControlsContainer}>
          <ControlButton
            onPress={() => {
              move(0, -500);
            }}
          >
            Up
          </ControlButton>
          <ControlButton
            onPress={() => {
              move(0, 500);
            }}
          >
            Down
          </ControlButton>
        </View>
        <ControlButton
          onPress={() => {
            move(500, 0);
          }}
        >
          Right
        </ControlButton>
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: { flex: 1 },
  systemButtonsContainer: {
    position: 'absolute',
    bottom: 100,
    left: 25,
    gap: 10,
  },
  dPadContainer: {
    position: 'absolute',
    flexDirection: 'row',
    bottom: 100,
    right: 25,
    gap: 10,
    alignItems: 'flex-end',
  },
  verticalControlsContainer: {
    gap: 10,
  },
  gameEngine: { flex: 1 },
});
