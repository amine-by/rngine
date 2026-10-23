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

#include "ObjectFit.hpp"
#include "Repeat.hpp"
#include <string>

namespace margelo::nitro::rngine {

struct Screen final {
public:
  double px SWIFT_PRIVATE;
  double py SWIFT_PRIVATE;
  double width SWIFT_PRIVATE;
  double height SWIFT_PRIVATE;
  std::string color SWIFT_PRIVATE;
  double asset SWIFT_PRIVATE;
  bool flipH SWIFT_PRIVATE;
  bool flipV SWIFT_PRIVATE;
  ObjectFit objectFit SWIFT_PRIVATE;
  bool clip SWIFT_PRIVATE;
  Repeat repeat SWIFT_PRIVATE;
  double progress SWIFT_PRIVATE;
  double speed SWIFT_PRIVATE;
  bool loop SWIFT_PRIVATE;

public:
  Screen() = default;
  explicit Screen(double px, double py, double width, double height,
                  std::string color, double asset, bool flipH, bool flipV,
                  ObjectFit objectFit, bool clip, Repeat repeat,
                  double progress, double speed, bool loop)
      : px(px), py(py), width(width), height(height), color(color),
        asset(asset), flipH(flipH), flipV(flipV), objectFit(objectFit),
        clip(clip), repeat(repeat), progress(progress), speed(speed),
        loop(loop) {}

public:
  friend bool operator==(const Screen &lhs, const Screen &rhs) = default;
};

} // namespace margelo::nitro::rngine

namespace margelo::nitro {

template <> struct JSIConverter<rngine::Screen> final {
  static inline rngine::Screen fromJSI(jsi::Runtime &runtime,
                                       const jsi::Value &arg) {
    jsi::Object obj = arg.asObject(runtime);
    return rngine::Screen(
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "px"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "py"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "width"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "height"))),
        JSIConverter<std::string>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "color"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "asset"))),
        JSIConverter<bool>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "flipH"))),
        JSIConverter<bool>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "flipV"))),
        JSIConverter<rngine::ObjectFit>::fromJSI(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "objectFit"))),
        JSIConverter<bool>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "clip"))),
        JSIConverter<rngine::Repeat>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "repeat"))),
        JSIConverter<double>::fromJSI(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "progress"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "speed"))),
        JSIConverter<bool>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "loop"))));
  }
  static inline jsi::Value toJSI(jsi::Runtime &runtime,
                                 const rngine::Screen &arg) {
    jsi::Object obj(runtime);
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "px"),
                    JSIConverter<double>::toJSI(runtime, arg.px));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "py"),
                    JSIConverter<double>::toJSI(runtime, arg.py));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "width"),
                    JSIConverter<double>::toJSI(runtime, arg.width));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "height"),
                    JSIConverter<double>::toJSI(runtime, arg.height));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "color"),
                    JSIConverter<std::string>::toJSI(runtime, arg.color));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "asset"),
                    JSIConverter<double>::toJSI(runtime, arg.asset));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "flipH"),
                    JSIConverter<bool>::toJSI(runtime, arg.flipH));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "flipV"),
                    JSIConverter<bool>::toJSI(runtime, arg.flipV));
    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "objectFit"),
        JSIConverter<rngine::ObjectFit>::toJSI(runtime, arg.objectFit));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "clip"),
                    JSIConverter<bool>::toJSI(runtime, arg.clip));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "repeat"),
                    JSIConverter<rngine::Repeat>::toJSI(runtime, arg.repeat));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "progress"),
                    JSIConverter<double>::toJSI(runtime, arg.progress));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "speed"),
                    JSIConverter<double>::toJSI(runtime, arg.speed));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "loop"),
                    JSIConverter<bool>::toJSI(runtime, arg.loop));
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
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "px"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "py"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "width"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "height"))))
      return false;
    if (!JSIConverter<std::string>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "color"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "asset"))))
      return false;
    if (!JSIConverter<bool>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "flipH"))))
      return false;
    if (!JSIConverter<bool>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "flipV"))))
      return false;
    if (!JSIConverter<rngine::ObjectFit>::canConvert(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "objectFit"))))
      return false;
    if (!JSIConverter<bool>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "clip"))))
      return false;
    if (!JSIConverter<rngine::Repeat>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "repeat"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "progress"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "speed"))))
      return false;
    if (!JSIConverter<bool>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "loop"))))
      return false;
    return true;
  }
};

} // namespace margelo::nitro
