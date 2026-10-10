#pragma once

#include <optional>
#if __has_include(<NitroModules/JSIConverter.hpp>)
#include <NitroModules/JSIConverter.hpp>
#else
#error NitroModules cannot be found! Are you sure you installed NitroModules properly?
#endif
#if __has_include(<NitroModules/NitroDefines.hpp>)
#include <NitroModules/NitroDefines.hpp>
#else
#error NitroModules cannot be found! Are you sure you installed NitroModules properly?
#endif
#if __has_include(<NitroModules/JSIHelpers.hpp>)
#include <NitroModules/JSIHelpers.hpp>
#else
#error NitroModules cannot be found! Are you sure you installed NitroModules properly?
#endif
#if __has_include(<NitroModules/PropNameIDCache.hpp>)
#include <NitroModules/PropNameIDCache.hpp>
#else
#error NitroModules cannot be found! Are you sure you installed NitroModules properly?
#endif

#include "Collision.hpp"
#include "Entity.hpp"
#include "Screen.hpp"
#include "World.hpp"
#include <vector>

namespace margelo::nitro::rngine {

struct SystemContext final {
public:
  std::optional<World> world SWIFT_PRIVATE;
  std::optional<Screen> screen SWIFT_PRIVATE;
  std::optional<std::vector<Entity>> entities SWIFT_PRIVATE;
  std::optional<std::vector<Collision>> collisions SWIFT_PRIVATE;

public:
  SystemContext() = default;
  explicit SystemContext(std::optional<World> world,
                         std::optional<Screen> screen,
                         std::optional<std::vector<Entity>> entities,
                         std::optional<std::vector<Collision>> collisions)
      : world(world), screen(screen), entities(entities),
        collisions(collisions) {}

public:
  friend bool operator==(const SystemContext &lhs,
                         const SystemContext &rhs) = default;
};

} // namespace margelo::nitro::rngine

namespace margelo::nitro {

template <> struct JSIConverter<rngine::SystemContext> final {
  static inline rngine::SystemContext fromJSI(jsi::Runtime &runtime,
                                              const jsi::Value &arg) {
    jsi::Object obj = arg.asObject(runtime);

    return rngine::SystemContext(
        JSIConverter<std::optional<rngine::World>>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "world"))),
        JSIConverter<std::optional<rngine::Screen>>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "screen"))),
        JSIConverter<std::optional<std::vector<rngine::Entity>>>::fromJSI(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "entities"))),
        JSIConverter<std::optional<std::vector<rngine::Collision>>>::fromJSI(
            runtime, obj.getProperty(runtime, PropNameIDCache::get(
                                                  runtime, "collisions"))));
  }

  static inline jsi::Value toJSI(jsi::Runtime &runtime,
                                 const rngine::SystemContext &arg) {
    jsi::Object obj(runtime);

    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "world"),
        JSIConverter<std::optional<rngine::World>>::toJSI(runtime, arg.world));

    obj.setProperty(runtime, PropNameIDCache::get(runtime, "screen"),
                    JSIConverter<std::optional<rngine::Screen>>::toJSI(
                        runtime, arg.screen));

    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "entities"),
        JSIConverter<std::optional<std::vector<rngine::Entity>>>::toJSI(
            runtime, arg.entities));

    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "collisions"),
        JSIConverter<std::optional<std::vector<rngine::Collision>>>::toJSI(
            runtime, arg.collisions));

    return obj;
  }

  static inline bool canConvert(jsi::Runtime &runtime,
                                const jsi::Value &value) {
    if (!value.isObject()) {
      return false;
    }

    jsi::Object obj = value.getObject(runtime);
    if (!nitro::isPlainObject(runtime, obj)) {
      return false;
    }

    if (!JSIConverter<std::optional<rngine::World>>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "world"))))
      return false;

    if (!JSIConverter<std::optional<rngine::Screen>>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "screen"))))
      return false;

    if (!JSIConverter<std::optional<std::vector<rngine::Entity>>>::canConvert(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "entities"))))
      return false;

    if (!JSIConverter<std::optional<std::vector<rngine::Collision>>>::
            canConvert(runtime,
                       obj.getProperty(runtime, PropNameIDCache::get(
                                                    runtime, "collisions"))))
      return false;

    return true;
  }
};

} // namespace margelo::nitro
