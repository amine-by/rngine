#pragma once

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

#include "CollisionPair.hpp"
#include "SystemContext.hpp"
#include <NitroModules/Promise.hpp>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace margelo::nitro::rngine {

struct System final {
public:
  std::optional<bool> world SWIFT_PRIVATE;
  std::optional<bool> screen SWIFT_PRIVATE;
  std::optional<std::vector<std::string>> entities SWIFT_PRIVATE;
  std::optional<std::vector<CollisionPair>> collisions SWIFT_PRIVATE;
  std::function<std::shared_ptr<Promise<void>>(const SystemContext &)> onTick
      SWIFT_PRIVATE;

public:
  System() = default;
  explicit System(
      std::optional<bool> world, std::optional<bool> screen,
      std::optional<std::vector<std::string>> entities,
      std::optional<std::vector<CollisionPair>> collisions,
      std::function<std::shared_ptr<Promise<void>>(const SystemContext &)>
          onTick)
      : world(world), screen(screen), entities(entities),
        collisions(collisions), onTick(onTick) {}
};

} // namespace margelo::nitro::rngine

namespace margelo::nitro {

template <> struct JSIConverter<rngine::System> final {
  using OnTick = std::function<std::shared_ptr<Promise<void>>(
      const rngine::SystemContext &)>;

  static inline OnTick makeOnTick(jsi::Runtime &runtime,
                                  const jsi::Value &value) {
    jsi::Function fn = value.asObject(runtime).asFunction(runtime);

    BorrowingReference<jsi::Function> shared;
    {
      JSICacheReference cache = JSICache::getOrCreateCache(runtime);
      shared = cache.makeShared(std::move(fn));
    }

    SyncJSCallback<void(const rngine::SystemContext &)> sync(runtime,
                                                             std::move(shared));
    auto dispatcher = Dispatcher::getRuntimeGlobalDispatcher(runtime);
    AsyncJSCallback<void(const rngine::SystemContext &)> async(std::move(sync),
                                                               dispatcher);

    return [async = std::move(async)](const rngine::SystemContext &ctx) {
      return async.call(ctx);
    };
  }

  static inline rngine::System fromJSI(jsi::Runtime &runtime,
                                       const jsi::Value &arg) {
    jsi::Object obj = arg.asObject(runtime);
    return rngine::System(
        JSIConverter<std::optional<bool>>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "world"))),
        JSIConverter<std::optional<bool>>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "screen"))),
        JSIConverter<std::optional<std::vector<std::string>>>::fromJSI(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "entities"))),
        JSIConverter<std::optional<std::vector<rngine::CollisionPair>>>::
            fromJSI(runtime,
                    obj.getProperty(
                        runtime, PropNameIDCache::get(runtime, "collisions"))),
        makeOnTick(runtime, obj.getProperty(runtime, PropNameIDCache::get(
                                                         runtime, "onTick"))));
  }

  static inline jsi::Value toJSI(jsi::Runtime &runtime,
                                 const rngine::System &arg) {
    jsi::Object obj(runtime);
    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "world"),
        JSIConverter<std::optional<bool>>::toJSI(runtime, arg.world));
    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "screen"),
        JSIConverter<std::optional<bool>>::toJSI(runtime, arg.screen));
    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "entities"),
        JSIConverter<std::optional<std::vector<std::string>>>::toJSI(
            runtime, arg.entities));
    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "collisions"),
        JSIConverter<std::optional<std::vector<rngine::CollisionPair>>>::toJSI(
            runtime, arg.collisions));
    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "onTick"),
        JSIConverter<std::function<std::shared_ptr<Promise<void>>(
            const rngine::SystemContext &)>>::toJSI(runtime, arg.onTick));
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
    if (!JSIConverter<std::optional<bool>>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "world"))))
      return false;
    if (!JSIConverter<std::optional<bool>>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "screen"))))
      return false;
    if (!JSIConverter<std::optional<std::vector<std::string>>>::canConvert(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "entities"))))
      return false;
    if (!JSIConverter<std::optional<std::vector<rngine::CollisionPair>>>::
            canConvert(runtime,
                       obj.getProperty(runtime, PropNameIDCache::get(
                                                    runtime, "collisions"))))
      return false;
    if (!JSIConverter<std::function<std::shared_ptr<Promise<void>>(
            const rngine::SystemContext &)>>::
            canConvert(runtime,
                       obj.getProperty(
                           runtime, PropNameIDCache::get(runtime, "onTick"))))
      return false;

    return true;
  }
};

} // namespace margelo::nitro
