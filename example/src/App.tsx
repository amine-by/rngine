import { useState } from 'react';
import { StyleSheet, View } from 'react-native';
import { GameEngine } from 'rngine';
import { ControlButton } from './components/ControlButton';

export default function App() {
  const [isPaused, setIsPaused] = useState(false);
  const onTogglePause = () => {
    setIsPaused((prev) => !prev);
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
        <ControlButton>Left</ControlButton>
        <View style={styles.verticalControlsContainer}>
          <ControlButton>Up</ControlButton>
          <ControlButton>Down</ControlButton>
        </View>
        <ControlButton>Right</ControlButton>
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
