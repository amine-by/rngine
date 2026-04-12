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
      <GameEngine isPaused={isPaused} style={styles.gameEngine} />
      <TouchableOpacity
        style={styles.pauseToggleButton}
        onPress={onTogglePause}
      >
        <Text>Toggle Pause</Text>
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
  },
  gameEngine: { flex: 1 },
});
