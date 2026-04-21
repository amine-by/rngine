#include "GameMethods.hpp"
#include "GameLoop.hpp"
#include <android/log.h>

namespace margelo::nitro::rngine {
void GameMethods::setP(const std::string &id, double px, double py) {
  auto *entity = ::rngine::GameLoop::getInstance().getEntityById(id);
  if (entity == nullptr) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "setP: entity not found for id=%s", id.c_str());
    return;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setP: entity found, setting px=%f py=%f", px, py);
  entity->px = px;
  entity->py = py;
}

void GameMethods::setV(const std::string &id, double vx, double vy) {
  auto *entity = ::rngine::GameLoop::getInstance().getEntityById(id);
  if (entity == nullptr) {
    __android_log_print(ANDROID_LOG_WARN, "GameMethods",
                        "setV: entity not found for id=%s", id.c_str());
    return;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods",
                      "setV: entity found, setting vx=%f vy=%f", vx, vy);
  entity->vx = vx;
  entity->vy = vy;
}
} // namespace margelo::nitro::rngine
