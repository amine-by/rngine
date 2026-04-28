export type Entity = {
  id: string;
  px: number;
  py: number;
  width: number;
  height: number;
  color: string;
  vx: number;
  vy: number;
};

export type System = {
  ids: string[];
  onTick: (entities: Entity[]) => void;
};
