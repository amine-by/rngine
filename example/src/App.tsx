import { useState } from 'react';
import { StyleSheet, View } from 'react-native';
import {
  GameEngine,
  configure,
  pause,
  resume,
  setVelocity,
  setPosition,
  spawn,
  despawn,
} from 'rngine';
import { ControlButton } from './components/ControlButton';

configure({
  tickRate: 60,
  world: {
    width: 800,
    height: 1000,
    color: '#ffffff',
  },
  entities: [
    {
      id: 'entity_1',
      px: 300,
      py: 300,
      width: 50,
      height: 50,
      color: '#00ff00',
      vx: 0,
      vy: 0,
    },
  ],
  systems: [
    {
      ids: ['entity'],
      onTick: (entities) => {
        console.log(entities);
      },
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
        width: 50,
        height: 50,
        color: '#ff0000',
        vx: 0,
        vy: 0,
      },
      {
        id: 'entity_3',
        px: 300,
        py: 500,
        width: 50,
        height: 50,
        color: '#0000ff',
        vx: 0,
        vy: 0,
      },
    ]);
  };

  const despawnAll = () => {
    if (isPaused) return;
    despawn('entity');
  };

  const move = (vx: number, vy: number) => {
    if (isPaused) return;
    setVelocity('entity', vx, vy);
  };

  const reposition = () => {
    if (isPaused) return;
    setPosition('entity_1', 300, 300);
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
