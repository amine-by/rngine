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

      const setShapeWithAnchor = (
        newWidth: number,
        newHeight: number,
        newAsset?: number
      ) => {
        entityUpdate.asset = newAsset;
        entityUpdate.shape = { width: newWidth, height: newHeight };
        entityUpdate.loop = true;
        entityUpdate.progress = 0;

        if ('height' in player.shape) {
          const heightDiff = newHeight - player.shape.height;
          entityUpdate.py = player.py - heightDiff / 2;
        }
      };

      if (!!collisions![0] && collisions![0].ny > 0) {
        if (
          playerState.canRefillJumps &&
          playerState.availableJumps < 2 &&
          player.asset !== Appearing
        ) {
          playerState.availableJumps = 2;
        }

        if (playerState.targetVx === 0) {
          if (player?.asset !== Idle && player.asset !== Appearing) {
            setShapeWithAnchor(21, 29, Idle);
          } else if (player?.asset === Appearing && player?.progress === 1) {
            entityUpdate.asset = Idle;
            entityUpdate.shape = { width: 21, height: 29 };
            entityUpdate.loop = true;
            entityUpdate.progress = 0;
          }
        } else if (player?.asset !== Run) {
          setShapeWithAnchor(23, 30, Run);
        }
      } else {
        playerState.canRefillJumps = true;

        if (
          player?.vy < 0 &&
          player?.asset !== Jump &&
          player?.asset !== Double_Jump
        ) {
          setShapeWithAnchor(21, 31, Jump);
        } else if (
          player?.vy > 0 &&
          player?.asset !== Fall &&
          player?.asset !== Double_Jump
        ) {
          setShapeWithAnchor(23, 29, Fall);
        } else if (player?.asset === Double_Jump && player?.progress === 1) {
          setShapeWithAnchor(23, 31, Jump);
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
