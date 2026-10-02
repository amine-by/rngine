export type Direction = 'UP' | 'DOWN' | 'LEFT' | 'RIGHT';

export type GameState = 'IDLE' | 'PLAYING' | 'PAUSED' | 'OVER';

type SnakeState = {
  direction: Direction;
  gameState: GameState;
};

export const snakeState: SnakeState = {
  direction: 'LEFT',
  gameState: 'IDLE',
};

type Listener = (state: GameState) => void;

const listeners = new Set<Listener>();

export function subscribeToGameState(listener: Listener) {
  listeners.add(listener);
  return () => listeners.delete(listener);
}

export function setGameState(state: GameState) {
  snakeState.gameState = state;
  listeners.forEach((listener) => listener(state));
}
