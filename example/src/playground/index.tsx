import { useEffect, useState } from 'react';
import { StyleSheet, View } from 'react-native';
import { GameEngine, configure, pause, resume, update, despawn } from 'rngine';
import { ControlButton } from './components/ControlButton';
import { useAssets } from '../AssetsContext';

export default function Playground() {
  const [isPaused, setIsPaused] = useState(true);
  const { getAssets } = useAssets();

  const { background_test_svg, test_lottie, test_svg_rect, test_svg_circle } =
    getAssets('Playground') ?? {};

  useEffect(() => {
    configure({
      world: { tickRate: 60, gy: 1000 },
      screen: {
        width: 800,
        height: 800,
        asset: background_test_svg,
      },
      entities: [
        {
          id: 'entity_1',
          px: 300,
          py: 100,
          color: '#ff0',
          shape: { width: 75, height: 105 },
          asset: test_lottie,
          mass: 5,
        },
        {
          id: 'entity_2',
          px: 400,
          py: 100,
          color: '#f00',
          shape: { width: 52, height: 84 },
          asset: test_svg_rect,
          mass: 5,
          vx: 10,
          ay: -1000,
        },
        {
          id: 'entity_3',
          px: 500,
          py: 100,
          color: '#00f',
          shape: { radius: 30 },
          asset: test_svg_circle,
          mass: 5,
          vx: -10,
          ay: -1000,
        },
        {
          id: 'ground',
          px: 400,
          py: 780,
          shape: { width: 800, height: 40 },
          color: '#654321',
        },
      ],
      systems: [
        {
          collisions: [{ a: 'entity', b: 'ground' }],
          onTick: (_, collisions) => {
            collisions.forEach(({ a, b, depth, nx, ny }) => {
              console.log(`a=${a} b=${b} depth=${depth} nx=${nx} ny=${ny}`);
            });
          },
        },
      ],
    });
  }, [background_test_svg, test_svg_rect, test_svg_circle, test_lottie]);

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

  const despawnAll = () => {
    if (isPaused) return;
    despawn('entity');
  };

  const jump = () => {
    update({ id: 'entity_1', vy: -600 });
  };

  const move = (ax: number) => {
    if (isPaused) return;
    update({ id: 'entity_1', ax, vx: 0 });
  };

  const reposition = () => {
    if (isPaused) return;

    update([
      { id: 'entity_1', px: 300, py: 100, vx: 0, vy: 0, ax: 0 },
      { id: 'entity_2', px: 400, py: 100, vy: 0 },
      { id: 'entity_3', px: 500, py: 100, vy: 0 },
    ]);
  };
  return (
    <View style={styles.container}>
      <GameEngine style={styles.gameEngine} />
      <View style={styles.systemButtonsContainer}>
        <ControlButton onPress={onTogglePause}>
          {isPaused ? 'Resume' : 'Pause'}
        </ControlButton>
        <ControlButton onPress={reposition}>Repo</ControlButton>
        <ControlButton onPress={despawnAll}>Despawn</ControlButton>
      </View>
      <View style={styles.dPadContainer}>
        <ControlButton
          onPress={() => {
            move(-100);
          }}
        >
          Left
        </ControlButton>
        <ControlButton
          onPress={() => {
            jump();
          }}
        >
          Jump
        </ControlButton>
        <ControlButton
          onPress={() => {
            move(100);
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
