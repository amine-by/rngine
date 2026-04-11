import { StyleSheet } from 'react-native';
import { GameEngine, createGameLoop } from 'rngine';

createGameLoop();

export default function App() {
  return <GameEngine style={styles.gameEngine} />;
}

const styles = StyleSheet.create({
  gameEngine: { flex: 1 },
});
