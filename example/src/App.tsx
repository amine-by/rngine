import { useState } from 'react';
import { StyleSheet, View } from 'react-native';
import { GameEngine, gameMethods } from 'rngine';
import { ControlButton } from './components/ControlButton';

export default function App() {
  const [isPaused, setIsPaused] = useState(false);
  const onTogglePause = () => {
    setIsPaused((prev) => !prev);
  };
  const move = (xv: number, yv: number) => {
    if (isPaused) return;
    gameMethods?.setV('entity1', xv, yv);
  };
  return (
    <View style={styles.container}>
      <GameEngine
        isPaused={isPaused}
        initialEntities={[
          {
            id: 'entity1',
            x: 300,
            y: 300,
            width: 50,
            height: 50,
            xv: 0,
            yv: 0,
          },
        ]}
        style={styles.gameEngine}
      />
      <ControlButton style={styles.pauseToggleButton} onPress={onTogglePause}>
        {isPaused ? 'Resume' : 'Pause'}
      </ControlButton>
      <View style={styles.controlsContainer}>
        <ControlButton
          onPress={() => {
            move(-100, 0);
          }}
        >
          Left
        </ControlButton>
        <View style={styles.verticalControlsContainer}>
          <ControlButton
            onPress={() => {
              move(0, -100);
            }}
          >
            Up
          </ControlButton>
          <ControlButton
            onPress={() => {
              move(0, 100);
            }}
          >
            Down
          </ControlButton>
        </View>
        <ControlButton
          onPress={() => {
            move(100, 0);
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
  pauseToggleButton: {
    position: 'absolute',
    top: 50,
    right: 25,
  },
  controlsContainer: {
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
