import {
  updateEntity,
  updateScreen,
  type Entity,
  type EntityUpdate,
  type ScreenUpdate,
  type System,
} from 'rngine';
import { getAssets } from '../../assets/assetsManager';
import { SCREEN_HEIGHT, SCREEN_WIDTH } from './consts';
import { playerState } from './state';

const { Idle, Appearing, Run, Jump, Fall, Double_Jump } =
  getAssets('Playground') ?? {};

export const systems: System[] = [
  {
    screen: true,
    collisions: [{ a: 'player', b: 'platform' }],
    entities: ['player'],
    onTick: ({ entities, collisions, screen }) => {
      const player = entities![0] as Entity;

      let entityUpdate: EntityUpdate = {
        id: 'player',
      };

      if (playerState.targetVx !== player?.vx) {
        entityUpdate.vx = playerState.targetVx;
      }

      let screenUpdate: ScreenUpdate = {};

      if (!!collisions![0] && collisions![0].ny > 0) {
        if (
          playerState.canRefillJumps &&
          playerState.availableJumps < 2 &&
          player.asset !== Appearing
        ) {
          playerState.availableJumps = 2;
        }

        if (
          playerState.targetVx === 0 &&
          ((player?.asset !== Idle && player.asset !== Appearing) ||
            (player?.asset === Appearing && player?.progress === 1))
        ) {
          entityUpdate.asset = Idle;
          entityUpdate.shape = { width: 21, height: 29 };
          entityUpdate.loop = true;
          entityUpdate.progress = 0;
        } else if (playerState.targetVx !== 0 && player?.asset !== Run) {
          entityUpdate.asset = Run;
          entityUpdate.shape = { width: 23, height: 30 };
          entityUpdate.loop = true;
          entityUpdate.progress = 0;
        }
      } else {
        playerState.canRefillJumps = true;

        if (
          typeof player?.vy !== 'undefined' &&
          !isNaN(player?.vy) &&
          player?.vy < 0 &&
          player?.asset !== Jump &&
          player?.asset !== Double_Jump
        ) {
          entityUpdate.asset = Jump;
          entityUpdate.shape = { width: 21, height: 31 };
          entityUpdate.loop = true;
          entityUpdate.progress = 0;
        } else if (
          typeof player?.vy !== 'undefined' &&
          !isNaN(player?.vy) &&
          player?.vy > 0 &&
          player?.asset !== Fall &&
          player?.asset !== Double_Jump
        ) {
          entityUpdate.asset = Fall;
          entityUpdate.shape = { width: 23, height: 29 };
          entityUpdate.loop = true;
          entityUpdate.progress = 0;
        } else if (player?.asset === Double_Jump && player?.progress === 1) {
          entityUpdate.asset = Jump;
          entityUpdate.shape = { width: 23, height: 31 };
          entityUpdate.loop = true;
          entityUpdate.progress = 0;
        }
      }

      if (player.px > SCREEN_WIDTH && screen!.px < SCREEN_WIDTH) {
        screenUpdate.px = SCREEN_WIDTH * 1.5;
      } else if (player.px < SCREEN_WIDTH && screen!.px > SCREEN_WIDTH) {
        screenUpdate.px = SCREEN_WIDTH * 0.5;
      }

      if (player.py > SCREEN_HEIGHT && screen!.py < SCREEN_HEIGHT) {
        screenUpdate.py = SCREEN_HEIGHT * 1.5;
      } else if (player.py < SCREEN_HEIGHT && screen!.py > SCREEN_HEIGHT) {
        screenUpdate.py = SCREEN_HEIGHT * 0.5;
      }

      if (
        typeof entityUpdate.vx !== 'undefined' ||
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
