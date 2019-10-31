# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(${JNI_LIBS_DIR}/includes)

set(JNI_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/main/cpp)

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.cpp
        src/main/cpp/Utils.c
        )

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp)

add_library(avcodec-57  # 库名字
            SHARED
            IMPORTED)
set_target_properties( avcodec-57
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/armeabi-v7a/libavcodec-57.so)



add_library(libavdevice-57  # 库名字
        SHARED
        IMPORTED)
set_target_properties( libavdevice-57
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/armeabi-v7a/libavdevice-57.so)



add_library(libavfilter-6  # 库名字
        SHARED
        IMPORTED)
set_target_properties( libavfilter-6
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/armeabi-v7a/libavfilter-6.so)



add_library(libavformat-57  # 库名字
        SHARED
        IMPORTED)
set_target_properties( libavformat-57
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/armeabi-v7a/libavformat-57.so)



add_library(libavutil-55  # 库名字
        SHARED
        IMPORTED)
set_target_properties( libavutil-55
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/armeabi-v7a/libavutil-55.so)



add_library(libpostproc-54  # 库名字
        SHARED
        IMPORTED)
set_target_properties( libpostproc-54
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/armeabi-v7a/libpostproc-54.so)



add_library(libswresample-2  # 库名字
        SHARED
        IMPORTED)
set_target_properties( libswresample-2
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/armeabi-v7a/libswresample-2.so)



add_library(libswscale-4  # 库名字
        SHARED
        IMPORTED)
set_target_properties( libswscale-4
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/armeabi-v7a/libswscale-4.so)



# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib
                        avcodec-57
        libavdevice-57
        libavfilter-6
        libavformat-57
        libavutil-55
        libpostproc-54
        libswresample-2
        libswscale-4
        android
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )