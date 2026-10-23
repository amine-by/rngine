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

namespace margelo::nitro::rngine {

struct World final {
public:
  double tickRate SWIFT_PRIVATE;
  double gx SWIFT_PRIVATE;
  double gy SWIFT_PRIVATE;

public:
  World() = default;
  explicit World(double tickRate, double gx, double gy)
      : tickRate(tickRate), gx(gx), gy(gy) {}

public:
  friend bool operator==(const World &lhs, const World &rhs) = default;
};

} // namespace margelo::nitro::rngine

namespace margelo::nitro {

template <> struct JSIConverter<rngine::World> final {
  static inline rngine::World fromJSI(jsi::Runtime &runtime,
                                      const jsi::Value &arg) {
    jsi::Object obj = arg.asObject(runtime);
    return rngine::World(
        JSIConverter<double>::fromJSI(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "tickRate"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "gx"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "gy"))));
  }

  static inline jsi::Value toJSI(jsi::Runtime &runtime,
                                 const rngine::World &arg) {
    jsi::Object obj(runtime);
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "tickRate"),
                    JSIConverter<double>::toJSI(runtime, arg.tickRate));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "gx"),
                    JSIConverter<double>::toJSI(runtime, arg.gx));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "gy"),
                    JSIConverter<double>::toJSI(runtime, arg.gy));
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
    if (!JSIConverter<double>::canConvert(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "tickRate"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "gx"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "gy"))))
      return false;
    return true;
  }
};

} // namespace margelo::nitro
