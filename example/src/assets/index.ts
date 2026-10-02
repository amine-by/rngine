import Food from './snake/food.svg';
import Head_Up from './snake/head_up.svg';
import Head_Left from './snake/head_left.svg';
import Body_Up_Left from './snake/body_up_left.svg';
import Body_Horizontal from './snake/body_horizontal.svg';
import Body_Vertical from './snake/body_vertical.svg';
import Tail_Up from './snake/tail_up.svg';
import Tail_Right from './snake/tail_right.svg';
import Background_Tile from './playground/background_tile.png';
import Appearing from './playground/appearing.json';
import Idle from './playground/idle.json';
import Run from './playground/run.json';
import Fall from './playground/fall.png';
import Jump from './playground/jump.png';
import Double_Jump from './playground/double_jump.json';

const ASSETS = {
  Snake: {
    Food,
    Head_Up,
    Head_Left,
    Body_Up_Left,
    Body_Horizontal,
    Body_Vertical,
    Tail_Up,
    Tail_Right,
  },
  Playground: {
    Background_Tile,
    Appearing,
    Idle,
    Run,
    Fall,
    Jump,
    Double_Jump,
  },
};

export default ASSETS;
