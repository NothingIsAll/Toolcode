cmake_minimum_required(VERSION 3.4.1)

find_path(GLES2_INCLUDE_DIR GLES2/gl2.h
    HINTS ${ANDROID_NDK})
find_library(GLES2_LIBRARY libGLESv2.so
    HINTS ${GLES2_INCLUDE_DIR}/../lib)

find_path(SL_INCLUDE_DIR SLES/OpenSLES.h
    HINTS ${ANDROID_NDK})
find_library(SL_LIBRARY libOpenSLES.so
    HINTS ${SL_INCLUDE_DIR}/../lib)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/main/cpp #h文件目录
    ${CMAKE_SOURCE_DIR}/libs/include
    ${GLES2_INCLUDE_DIR}
    ${SL_INCLUDE_DIR}
)
add_library(
            avcodec
            SHARED
            IMPORTED
            )
add_library(
            avfilter
            SHARED
            IMPORTED
             )
add_library(
            avformat
            SHARED
            IMPORTED
            )
add_library(
            avutil
            SHARED
            IMPORTED
            )
add_library(
            swresample
            SHARED
            IMPORTED
            )
add_library(
            swscale
            SHARED
            IMPORTED
            )
add_library(
            fdk-aac
            SHARED
            IMPORTED
            )
add_library( # Sets the name of the library.
             jni-native
             SHARED
             src/main/cpp/FFmpeg.cpp
             src/main/cpp/GlView.cpp
             src/main/cpp/XPlayer.cpp
             src/main/cpp/GlUtils.cpp
             src/main/cpp/Opensl_io.cpp
              )
set_target_properties(
    avcodec
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavcodec.so
    )
set_target_properties(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavfilter.so
        )
set_target_properties(
            avformat
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavformat.so
            )
set_target_properties(
            avutil
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavutil.so
            )
set_target_properties(
            swresample
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libswresample.so
             )
set_target_properties(
            swscale
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libswscale.so
             )
set_target_properties(
            fdk-aac
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libfdk-aac.so
            )
find_library( # Sets the name of the path variable.
              log-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


target_link_libraries( # Specifies the target library.
                     jni-native
                     fdk-aac
                     avcodec
                     avfilter
                     avformat
                     avutil
                     swresample
                     swscale
                     ${GLES2_LIBRARY}
                     ${SL_LIBRARY}
                     ${log-lib}
                     )