cmake_minimum_required(VERSION 2.8)

#if (${HOBOT_ANDROID_64})
#    include(cmake/hobot_util_android64.cmake)
#else ()
#    include(cmake/hobot_util.cmake)
#endif()
#project(misoundbarunderlyingsdk)
project(kws_ztx)

option(RECORD_DETECTION "Open recording mode" OFF)
if(${ALSA_RECORD_DETECTION})
    add_definitions(-DALSA_RECORD_MODE)
    set(alsa_record_include
        ${PROJECT_SOURCE_DIR}/src/record/
        ${PROJECT_SOURCE_DIR}/third_party/alsa/)
    set(alsa_record_src "src/record/speech_record_thread.cpp")
    set(alsa_record_lib asound)
    message("record: ============= alsa record_mode ============")
endif()
message("config types: ${CMAKE_CONFIGURATION_TYPES}")
set(toolchain ${toolchain} CACHE STRING "this is inner test")
message(STATUS "toolchain is ${toolchain}")
if(CMAKE_BUILD_TYPE)
    message(STATUS "cmake build type : ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "cmake build type undefine set default Release")
    set(CMAKE_BUILD_TYPE Release)
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -Wl,--no-warn-mismatch -fpermissive")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

add_definitions(-DSAVEFILES)

if (${PLATFORM_ANDROID})
    message("platform: ${PLATFORM_ANDROID}")
    if (${HOBOT_ANDROID_64})
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -pie -fPIE")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -pie -fPIE")
        message(STATUS "used android 64")
    else ()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -marm -mfpu=neon -Ofast -pie -fPIE")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -marm -mfpu=neon -Ofast -pie -fPIE")
        message(STATUS "used android 32")
    endif ()
endif()

if(${PLATFORM_ANDROID})
    if(${HOBOT_ANDROID_64})
        message(STATUS "used android 64")
    else ()
        message(STATUS "used android 32")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--version-script=${PROJECT_SOURCE_DIR}/symbol.lst -Wl,--no-undefined -Wl,--no-warn-mismatch -Wl,--version-script=${PROJECT_SOURCE_DIR}/symbol.lst")
        set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${PROJECT_SOURCE_DIR}/symbol.lst -Wl,--no-undefined -Wl,--no-warn-mismatch -Wl,--version-script=${PROJECT_SOURCE_DIR}/symbol.lst")
    endif ()
else ()
    set (CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined -Wl,--no-warn-mismatch")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${PROJECT_SOURCE_DIR}/symbol.lst -Wl,--no-undefined -Wl,--no-warn-mismatch")
endif ()

if((${CMAKE_BUILD_TYPE} MATCHES "MinSizeRel") OR (${CMAKE_BUILD_TYPE} MATCHES "Release"))
    message(STATUS "open strip")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-s")
else()
    message(STATUS "Debug version set save files")
endif()

if(${SECURE_VER})
    add_definitions(-DENABLE_SECURE)
    message(STATUS "add hss")
    set(SECURE_LIB hss uuid hcrypto curl wolfssl)
endif()

if(${debug_hobot})
    message(STATUS "Debug HOBOT")
    add_definitions(-Ddebug_hobot)
endif()

add_definitions(-D__STDC_LIMIT_MACROS)
if (PLATFORM_WIN)
    add_definitions(-DHR_WIN)
elseif (PLATFORM_LINUX)
    add_definitions(-DHR_POSIX)
elseif (PLATFORM_MAC)
    add_definitions(-DHR_POSIX)
elseif (PLATFORM_ANDROID)
    add_definitions(-DHR_POSIX)
endif ()


include_directories(
        ${PROJECT_SOURCE_DIR}/
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_SOURCE_DIR}/src    
        ${PROJECT_SOURCE_DIR}/third_party/util/
        ${PROJECT_SOURCE_DIR}/third_party/
        ${PROJECT_SOURCE_DIR}/third_party/tensorflow/include/
        ${PROJECT_SOURCE_DIR}/src/engine
        ${PROJECT_SOURCE_DIR}/src/util
        ${PROJECT_SOURCE_DIR}/src/util/fft_resample
        ${PROJECT_SOURCE_DIR}/src/decoder
        ${PROJECT_SOURCE_DIR}/src/frontEnd_DSP
        ${PROJECT_SOURCE_DIR}/headfile
        ${alsa_record_include}
)

link_directories(
        ${PROJECT_SOURCE_DIR}/build/
        ${PROJECT_SOURCE_DIR}/lib/
)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)


file(GLOB SOURCE_SRC 
        "src/*.cpp"
        "src/engine/*.cpp"
        "src/engine/*.c"
        "src/*.c"
        "src/*.cc"
        "src/*.hpp"
        "src/util/*.cpp"
        "src/util/*.c"
        "src/util/fft_resample/*.cpp"
        "src/util/fft_resample/*.cc"
        "third_party/util/*.cpp"
        "third_party/util/*.c"
        "src/decoder/*.cpp"
        "src/decoder/*.cc"
        "src/frontEnd_DSP/*.cpp"
        "src/kwslog/*.cpp"
        ${alsa_record_src}
        )


set(TARGET_LINK_LINUX_LIBS  
    tensorflow_cc
    tensorflow_framework
    ${alsa_record_lib}
    pthread)

set(TARGET_LINK_ANDROID_LIBS 
    ${alsa_record_lib}
    log m z)


add_library(kws SHARED ${SOURCE_SRC} ${SOURCE_INC})
set(TARGET_LINK_LIBS ${TARGET_LINK_LINUX_LIBS})

if(PLATFORM_LINUX)
    set(TARGET_LINK_LIBS ${TARGET_LINK_LINUX_LIBS})
elseif(PLATFORM_ANDROID)
    set(TARGET_LINK_LIBS ${TARGET_LINK_ANDROID_LIBS})
endif()


target_link_libraries(kws ${TARGET_LINK_LIBS})

# 每次打包都会打印出动态库和静态库
add_library(kws_static STATIC ${SOURCE_SRC} ${SOURCE_INC})
target_link_libraries(kws_static ${TARGET_LINK_LIBS})
set_target_properties(kws_static PROPERTIES OUTPUT_NAME kws)

add_subdirectory(example)
if(${ALSA_RECORD_DETECTION})
    add_subdirectory(record_demo)
endif()
set(OUTPUT_ROOT ${PROJECT_SOURCE_DIR}/output)

set(MY_OUTPUT_ROOT ${OUTPUT_ROOT}/kws_sdk)
# SDK lib
install(TARGETS kws
        DESTINATION ${MY_OUTPUT_ROOT}/lib)

# config
install(DIRECTORY ${PROJECT_SOURCE_DIR}/conf/
    DESTINATION ${MY_OUTPUT_ROOT}/kws/)

install(FILES ${PROJECT_SOURCE_DIR}/build/example/kwstest
        DESTINATION ${MY_OUTPUT_ROOT}/bin/)

      # DESTINATION ${MY_OUTPUT_ROOT}/kws/)

# SDK HEADFILE
install(FILES ${PROJECT_SOURCE_DIR}/include/kws_sdk.h
        DESTINATION ${MY_OUTPUT_ROOT}/include/)

# SDK  MODEL
install(DIRECTORY ${PROJECT_SOURCE_DIR}/model/
        DESTINATION ${MY_OUTPUT_ROOT}/kws/)

# lib
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/lib/
#        DESTINATION ${MY_OUTPUT_ROOT}/lib/)

install(FILES ${PROJECT_SOURCE_DIR}/audio_tool/test.sh
        DESTINATION ${MY_OUTPUT_ROOT}/../)
