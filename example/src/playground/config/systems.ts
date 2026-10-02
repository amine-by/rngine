import {
  updateEntity,
  updateScreen,
  type EntityUpdate,
  type System,
} from 'rngine';
import type { ScreenUpdate } from '../../../../src/nativeTypes';
import { getAssets } from '../../assets/assetsManager';
import { SCREEN_HEIGHT, SCREEN_WIDTH } from './consts';
import { playerState } from './state';

const { Idle, Appearing, Run, Jump, Fall, Double_Jump } =
  getAssets('Playground') ?? {};

export const systems: System[] = [
  {
    collisions: [{ a: 'player', b: 'platform' }],
    entities: ['player'],
    onTick: ({ entities, collisions, screen }) => {
      const player = entities[0];

      if (!player) {
        return;
      }

      let entityUpdate: EntityUpdate = {
        id: 'player',
        loop: true,
        progress: 0,
      };

      let screenUpdate: ScreenUpdate = {};

      if (collisions[0] && collisions[0].ny > 0) {
        if (playerState.availableJumps < 2) {
          playerState.availableJumps = 2;
        }
        if (
          player.vx === 0 &&
          ((player?.asset !== Idle && player.asset !== Appearing) ||
            (player?.asset === Appearing && player?.progress === 1))
        ) {
          entityUpdate.asset = Idle;
          entityUpdate.shape = { width: 21, height: 29 };
        } else if (player.vx !== 0 && player?.asset !== Run) {
          entityUpdate.asset = Run;
          entityUpdate.shape = { width: 23, height: 30 };
        }
      } else {
        if (
          typeof player?.vy !== 'undefined' &&
          !isNaN(player?.vy) &&
          player?.vy < 0 &&
          player?.asset !== Jump &&
          player?.asset !== Double_Jump
        ) {
          entityUpdate.asset = Jump;
          entityUpdate.shape = { width: 21, height: 31 };
        } else if (
          typeof player?.vy !== 'undefined' &&
          !isNaN(player?.vy) &&
          player?.vy > 0 &&
          player?.asset !== Fall &&
          player?.asset !== Double_Jump
        ) {
          entityUpdate.asset = Fall;
          entityUpdate.shape = { width: 23, height: 29 };
        } else if (player?.asset === Double_Jump && player?.progress === 1) {
          entityUpdate.asset = Jump;
          entityUpdate.shape = { width: 23, height: 31 };
        }
      }

      if (player.px > SCREEN_WIDTH && screen.px < SCREEN_WIDTH) {
        screenUpdate.px = SCREEN_WIDTH * 1.5;
      } else if (player.px < SCREEN_WIDTH && screen.px > SCREEN_WIDTH) {
        screenUpdate.px = SCREEN_WIDTH * 0.5;
      }

      if (player.py > SCREEN_HEIGHT && screen.py < SCREEN_HEIGHT) {
        screenUpdate.py = SCREEN_HEIGHT * 1.5;
      } else if (player.py < SCREEN_HEIGHT && screen.py > SCREEN_HEIGHT) {
        screenUpdate.py = SCREEN_HEIGHT * 0.5;
      }

      if (
        typeof entityUpdate.asset !== 'undefined' ||
        typeof entityUpdate.shape !== 'undefined'
      ) {
        updateEntity(entityUpdate);
      }

      if (
        typeof screenUpdate.px !== 'undefined' ||
        typeof screenUpdate.py !== 'undefined'
      ) {
        updateScreen(screenUpdate);
      }
    },
  },
];
