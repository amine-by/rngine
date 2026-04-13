#pragma once

namespace rngine {
static constexpr int ENTITY_ID_SIZE = 37;
static constexpr int ENTITY_BUFFER_SIZE = ENTITY_ID_SIZE + sizeof(double) * 4;

struct Entity {
  char id[ENTITY_ID_SIZE];
  double x;
  double y;
  double width;
  double height;
};
} // namespace rngine
