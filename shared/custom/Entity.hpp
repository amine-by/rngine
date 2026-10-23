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

#include "Circle.hpp"
#include "ObjectFit.hpp"
#include "Rect.hpp"
#include "Repeat.hpp"
#include <string>
#include <variant>

namespace margelo::nitro::rngine {
struct Entity final {
public:
  std::string id SWIFT_PRIVATE;
  double px SWIFT_PRIVATE;
  double py SWIFT_PRIVATE;
  std::variant<Rect, Circle> shape SWIFT_PRIVATE;
  double angle SWIFT_PRIVATE;
  bool isSensor SWIFT_PRIVATE;
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
  double vx SWIFT_PRIVATE;
  double vy SWIFT_PRIVATE;
  double ax SWIFT_PRIVATE;
  double ay SWIFT_PRIVATE;
  double mass SWIFT_PRIVATE;

public:
  Entity() = default;
  explicit Entity(std::string id, double px, double py,
                  std::variant<Rect, Circle> shape, double angle, bool isSensor,
                  std::string color, double asset, bool flipH, bool flipV,
                  ObjectFit objectFit, bool clip, Repeat repeat,
                  double progress, double speed, bool loop, double vx,
                  double vy, double ax, double ay, double mass)
      : id(id), px(px), py(py), shape(shape), angle(angle), isSensor(isSensor),
        color(color), asset(asset), flipH(flipH), flipV(flipV),
        objectFit(objectFit), clip(clip), repeat(repeat), progress(progress),
        speed(speed), loop(loop), vx(vx), vy(vy), ax(ax), ay(ay), mass(mass) {}

public:
  friend bool operator==(const Entity &lhs, const Entity &rhs) = default;
};

} // namespace margelo::nitro::rngine

namespace margelo::nitro {

template <> struct JSIConverter<rngine::Entity> final {
  static inline rngine::Entity fromJSI(jsi::Runtime &runtime,
                                       const jsi::Value &arg) {
    jsi::Object obj = arg.asObject(runtime);
    return rngine::Entity(
        JSIConverter<std::string>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "id"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "px"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "py"))),
        JSIConverter<std::variant<rngine::Rect, rngine::Circle>>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "shape"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "angle"))),
        JSIConverter<bool>::fromJSI(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "isSensor"))),
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
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "loop"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "vx"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "vy"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "ax"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "ay"))),
        JSIConverter<double>::fromJSI(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "mass"))));
  }

  static inline jsi::Value toJSI(jsi::Runtime &runtime,
                                 const rngine::Entity &arg) {
    jsi::Object obj(runtime);
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "id"),
                    JSIConverter<std::string>::toJSI(runtime, arg.id));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "px"),
                    JSIConverter<double>::toJSI(runtime, arg.px));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "py"),
                    JSIConverter<double>::toJSI(runtime, arg.py));
    obj.setProperty(
        runtime, PropNameIDCache::get(runtime, "shape"),
        JSIConverter<std::variant<rngine::Rect, rngine::Circle>>::toJSI(
            runtime, arg.shape));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "angle"),
                    JSIConverter<double>::toJSI(runtime, arg.angle));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "isSensor"),
                    JSIConverter<bool>::toJSI(runtime, arg.isSensor));
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
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "vx"),
                    JSIConverter<double>::toJSI(runtime, arg.vx));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "vy"),
                    JSIConverter<double>::toJSI(runtime, arg.vy));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "ax"),
                    JSIConverter<double>::toJSI(runtime, arg.ax));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "ay"),
                    JSIConverter<double>::toJSI(runtime, arg.ay));
    obj.setProperty(runtime, PropNameIDCache::get(runtime, "mass"),
                    JSIConverter<double>::toJSI(runtime, arg.mass));
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
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "id"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "px"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "py"))))
      return false;
    if (!JSIConverter<std::variant<rngine::Rect, rngine::Circle>>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "shape"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "angle"))))
      return false;
    if (!JSIConverter<bool>::canConvert(
            runtime, obj.getProperty(
                         runtime, PropNameIDCache::get(runtime, "isSensor"))))
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
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "vx"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "vy"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "ax"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "ay"))))
      return false;
    if (!JSIConverter<double>::canConvert(
            runtime,
            obj.getProperty(runtime, PropNameIDCache::get(runtime, "mass"))))
      return false;
    return true;
  }
};

} // namespace margelo::nitro
