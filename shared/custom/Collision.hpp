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

#include <string>

namespace margelo::nitro::rngine {

struct Collision final {
public:
  std::string a SWIFT_PRIVATE;
  std::string b SWIFT_PRIVATE;
  double depth SWIFT_PRIVATE;
  double nx SWIFT_PRIVATE;
  double ny SWIFT_PRIVATE;

public:
  Collision() = default;
  explicit Collision(std::string a, std::string b, double depth, double nx,
                     double ny)
      : a(a), b(b), depth(depth), nx(nx), ny(ny) {}

public:
  friend bool operator==(const Collision &lhs, const Collision &rhs) = default;
};

} // namespace margelo::nitro::rngine

namespace margelo::nitro {

template <> struct JSIConverter<rngine::Collision> final {

  static inline rngine::Collision fromJSI(jsi::Runtime &runtime,
                                          const jsi::Value &arg) {
    jsi::Object obj = arg.asObject(runtime);

    return rngine::Collision(
        JSIConverter<std::string>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "a"))),
        JSIConverter<std::string>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "b"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "depth"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "nx"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "ny"))));
  }

  static inline jsi::Value toJSI(jsi::Runtime &runtime,
                                 const rngine::Collision &arg) {
    jsi::Object obj(runtime);

    obj.setProperty(runtime, PropNameIDCache::get(runtime, "a"),
                    JSIConverter<std::string>::toJSI(runtime, arg.a));

    obj.setProperty(runtime, PropNameIDCache::get(runtime, "b"),
                    JSIConverter<std::string>::toJSI(runtime, arg.b));

    obj.setProperty(runtime, PropNameIDCache::get(runtime, "depth"),
                    JSIConverter<double>::toJSI(runtime, arg.depth));

    obj.setProperty(runtime, PropNameIDCache::get(runtime, "nx"),
                    JSIConverter<double>::toJSI(runtime, arg.nx));

    obj.setProperty(runtime, PropNameIDCache::get(runtime, "ny"),
                    JSIConverter<double>::toJSI(runtime, arg.ny));

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

    if (!JSIConverter<std::string>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "a"))))
      return false;

    if (!JSIConverter<std::string>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "b"))))
      return false;

    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "depth"))))
      return false;

    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "nx"))))
      return false;

    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "ny"))))
      return false;

    return true;
  }
};
} // namespace margelo::nitro
