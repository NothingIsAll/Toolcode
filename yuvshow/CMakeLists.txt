# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.
find_path(GLES2_INCLUDE_DIR GLES2/gl2.h
    HINTS ${ANDROID_NDK})              #找到opengl头文件路径
find_library(GLES2_LIBRARY libGLESv2.so
    HINTS ${GLES2_INCLUDE_DIR}/../lib) #找到opengl库文件路径

find_library(log-lib
             log)
set(opengl-bil GLESv2)
set(opengl-bil1 GLESv1_CM)
set(jnigraph-bli jnigraphics)
include_directories(./include/libyuv
                    ./include/libyuv/libyuv
                    ${GLES2_INCLUDE_DIR})

link_directories(./)





add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             native-lib.cpp
             NativeRender.cpp)


target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK
                       GLESv2
                       GLESv1_CM
                       jnigraphics
                       #${GLES2_LIBRARY}
                       ${log-lib} 
					   yuv)



#include_directories(
#    ${CMAKE_SOURCE_DIR}/src/main/cpp   #此处忽略
#    ${CMAKE_SOURCE_DIR}/libs/include   #此处忽略
#    ${GLES2_INCLUDE_DIR}               #把头文件路径添加进来
#)
#target_link_libraries(
#                     jni-native        #此处忽略
#                     ${GLES2_LIBRARY}  #把opengl库文件添加进来
#                     ${log-lib}        #此处忽略
#                     )