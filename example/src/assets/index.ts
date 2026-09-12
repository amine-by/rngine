import food from './snake/food.svg';
import head_up from './snake/head_up.svg';
import head_left from './snake/head_left.svg';
import body_up_left from './snake/body_up_left.svg';
import body_horizontal from './snake/body_horizontal.svg';
import body_vertical from './snake/body_vertical.svg';
import tail_up from './snake/tail_up.svg';
import tail_right from './snake/tail_right.svg';
import Background_Test_Svg from './playground/background_test_svg.svg';
import Idle from './playground/idle.json';
import Run from './playground/run.json';
import Fall from './playground/fall.png';
import Jump from './playground/jump.png';

const ASSETS = {
  Snake: {
    food,
    head_up,
    head_left,
    body_up_left,
    body_horizontal,
    body_vertical,
    tail_up,
    tail_right,
  },
  Playground: {
    Background_Test_Svg,
    Idle,
    Run,
    Fall,
    Jump,
  },
};

export default ASSETS;
