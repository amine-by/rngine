import { useState } from 'react';
import { StyleSheet, Text, TouchableOpacity, View } from 'react-native';
import { GameEngine } from 'rngine';

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
            x: 100,
            y: 100,
            width: 50,
            height: 50,
            xv: 0,
            yv: 0,
          },
          {
            id: 'entity2',
            x: 200,
            y: 100,
            width: 50,
            height: 50,
            xv: 100,
            yv: 0,
          },
          {
            id: 'entity3',
            x: 100,
            y: 200,
            width: 50,
            height: 50,
            xv: 0,
            yv: 100,
          },
          {
            id: 'entity4',
            x: 200,
            y: 200,
            width: 50,
            height: 50,
            xv: 100,
            yv: 100,
          },
        ]}
        style={styles.gameEngine}
      />
      <TouchableOpacity
        style={styles.pauseToggleButton}
        onPress={onTogglePause}
      >
        <Text>{isPaused ? 'Resume' : 'Pause'}</Text>
      </TouchableOpacity>
    </View>
  );
}

const styles = StyleSheet.create({
  container: { flex: 1 },
  pauseToggleButton: {
    backgroundColor: '#fff',
    position: 'absolute',
    left: '50%',
    top: '50%',
    transform: [{ translateX: '-50%' }, { translateY: '-50%' }],
  },
  gameEngine: { flex: 1 },
});
