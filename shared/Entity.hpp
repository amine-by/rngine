#pragma once

namespace rngine {
static constexpr int ENTITY_ID_SIZE = 40;
static constexpr int ENTITY_BUFFER_SIZE = ENTITY_ID_SIZE + sizeof(double) * 6;

struct Entity {
  char id[ENTITY_ID_SIZE];
  double px;
  double py;
  double width;
  double height;
  double vx;
  double vy;
};
} // namespace rngine
