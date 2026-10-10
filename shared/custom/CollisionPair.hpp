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

struct CollisionPair final {
public:
  std::string a SWIFT_PRIVATE;
  std::string b SWIFT_PRIVATE;

public:
  CollisionPair() = default;
  explicit CollisionPair(std::string a, std::string b) : a(a), b(b) {}

public:
  friend bool operator==(const CollisionPair &lhs,
                         const CollisionPair &rhs) = default;
};

} // namespace margelo::nitro::rngine

namespace margelo::nitro {

template <> struct JSIConverter<rngine::CollisionPair> final {

  static inline rngine::CollisionPair fromJSI(jsi::Runtime &runtime,
                                              const jsi::Value &arg) {
    jsi::Object obj = arg.asObject(runtime);

    return rngine::CollisionPair(
        JSIConverter<std::string>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "a"))),
        JSIConverter<std::string>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "b"))));
  }

  static inline jsi::Value toJSI(jsi::Runtime &runtime,
                                 const rngine::CollisionPair &arg) {
    jsi::Object obj(runtime);

    obj.setProperty(runtime, PropNameIDCache::get(runtime, "a"),
                    JSIConverter<std::string>::toJSI(runtime, arg.a));

    obj.setProperty(runtime, PropNameIDCache::get(runtime, "b"),
                    JSIConverter<std::string>::toJSI(runtime, arg.b));

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

    return true;
  }
};

} // namespace margelo::nitro
