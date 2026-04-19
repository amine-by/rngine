#include "GameMethods.hpp"
#include "GameLoop.hpp"
#include <android/log.h>

namespace margelo::nitro::rngine {
void GameMethods::setV(const std::string &id, double xv, double yv) {
  auto *entity = ::rngine::GameLoop::getInstance().getEntityById(id);
  if (entity == nullptr) {
      __android_log_print(ANDROID_LOG_WARN, "GameMethods", "setV: entity not found for id=%s", id.c_str());
      return;
  }
  __android_log_print(ANDROID_LOG_INFO, "GameMethods", "setV: entity found, setting xv=%f yv=%f", xv, yv);
  entity->xv = xv;
  entity->yv = yv;
}
} // namespace margelo::nitro::rngine
