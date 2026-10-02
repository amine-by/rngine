import { CELL, COLS, ROWS } from './consts';

export const screen = {
  px: (CELL * COLS) / 2,
  py: (CELL * ROWS) / 2,
  width: CELL * COLS,
  height: CELL * ROWS,
  color: '#1a1a1a',
};
