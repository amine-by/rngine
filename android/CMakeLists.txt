project(rngine)
cmake_minimum_required(VERSION 3.9.0)

set(PACKAGE_NAME rngine)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)
set(SKIA_ROOT ${CMAKE_SOURCE_DIR}/../shared/skia)
set(SKIA_LIBS_ROOT ${CMAKE_SOURCE_DIR}/../node_modules/react-native-skia-android/libs/${ANDROID_ABI})

add_library(skia STATIC IMPORTED)
set_property(TARGET skia PROPERTY IMPORTED_LOCATION "${SKIA_LIBS_ROOT}/libskia.a")

add_library(skia_svg STATIC IMPORTED)
set_property(TARGET skia_svg PROPERTY IMPORTED_LOCATION "${SKIA_LIBS_ROOT}/libsvg.a")

add_library(skia_skottie STATIC IMPORTED)
set_property(TARGET skia_skottie PROPERTY IMPORTED_LOCATION "${SKIA_LIBS_ROOT}/libskottie.a")

add_library(skia_sksg STATIC IMPORTED)
set_property(TARGET skia_sksg PROPERTY IMPORTED_LOCATION "${SKIA_LIBS_ROOT}/libsksg.a")

add_library(skia_skshaper STATIC IMPORTED)
set_property(TARGET skia_skshaper PROPERTY IMPORTED_LOCATION "${SKIA_LIBS_ROOT}/libskshaper.a")

add_library(skia_skunicode_core STATIC IMPORTED)
set_property(TARGET skia_skunicode_core PROPERTY IMPORTED_LOCATION "${SKIA_LIBS_ROOT}/libskunicode_core.a")

add_library(skia_skunicode_icu STATIC IMPORTED)
set_property(TARGET skia_skunicode_icu PROPERTY IMPORTED_LOCATION "${SKIA_LIBS_ROOT}/libskunicode_icu.a")

add_library(skia_jsonreader STATIC IMPORTED)
set_property(TARGET skia_jsonreader PROPERTY IMPORTED_LOCATION "${SKIA_LIBS_ROOT}/libjsonreader.a")

# Define C++ library and add all sources
add_library(
        ${PACKAGE_NAME}
        SHARED
        src/main/cpp/cpp-adapter.cpp
        src/main/cpp/game-view.cpp
        ../shared/GameLoop.cpp
        ../shared/GameRenderer.cpp
        ../shared/GameMethods.cpp
        ../shared/ColorUtils.cpp
        ../shared/CollisionUtils.cpp
)

# Add Nitrogen specs :)
include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/rngine+autolinking.cmake)

# Set up local includes
include_directories(
        "src/main/cpp"
        "../shared"
        "${SKIA_ROOT}"
)

find_library(LOG_LIB log)

# Link all libraries together
target_link_libraries(
        ${PACKAGE_NAME}
        ${LOG_LIB}
        android # <-- Android core
        skia_skottie
        skia_sksg
        skia_svg
        skia_skshaper
        skia_skunicode_core
        skia_skunicode_icu
        skia_jsonreader
        skia
        jnigraphics
        EGL
        GLESv3
)
