set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -Wl,--no-warn-mismatch -fpermissive")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

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
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined -Wl,--no-warn-mismatch")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${PROJECT_SOURCE_DIR}/symbol.lst -Wl,--no-undefined -Wl,--no-warn-mismatch")
endif ()

if((${CMAKE_BUILD_TYPE} MATCHES "MinSizeRel") OR (${CMAKE_BUILD_TYPE} MATCHES "Release"))
    message(STATUS "open strip")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-s")
else()
    message(STATUS "Debug version set save files")
endif()

include_directories(
        ${PROJECT_SOURCE_DIR}/include/
)
link_directories(
        ${PROJECT_SOURCE_DIR}/build
)

set(SOURCE_FILE
        ${PROJECT_SOURCE_DIR}/record_demo/record.cpp
        ${PROJECT_SOURCE_DIR}/src/util/stringHandle.cpp
        ${PROJECT_SOURCE_DIR}/src/record/speech_record_thread.cpp
        ${PROJECT_SOURCE_DIR}/src/kwslog/log.cpp
        )

add_executable(record ${SOURCE_FILE})
target_link_libraries(record pthread asound)
