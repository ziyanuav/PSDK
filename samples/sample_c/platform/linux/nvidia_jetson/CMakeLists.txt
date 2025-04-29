cmake_minimum_required(VERSION 3.5)
project(ziyan_sdk_demo_on_jetson C)

set(CMAKE_C_FLAGS "-pthread -std=gnu99")
set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
add_definitions(-D_GNU_SOURCE)


#Strengthen Compilation Warnings - Treat All Warnings as Errors
#Purpose: Minimize potential issues caused by syntax and compiler-specific features
# add_definitions(-Werror)
# add_definitions(-Wno-error=implicit-function-declaration)


if (NOT USE_SYSTEM_ARCH)
    add_definitions(-DSYSTEM_ARCH_LINUX)
endif ()

if (BUILD_TEST_CASES_ON MATCHES TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
endif ()

set(PACKAGE_NAME payloadsdk)

## "uname -m" to auto distinguish Manifold2-G or Manifold2-C
execute_process(COMMAND uname -m
        OUTPUT_VARIABLE DEVICE_SYSTEM_ID)

if (DEVICE_SYSTEM_ID MATCHES x86_64)
    set(TOOLCHAIN_NAME x86_64-linux-gnu)
    add_definitions(-DPLATFORM_ARCH_x86_64=1)
elseif (DEVICE_SYSTEM_ID MATCHES aarch64)
    set(TOOLCHAIN_NAME aarch64-linux-gnu)
    add_definitions(-DPLATFORM_ARCH_aarch64=1)
elseif (DEVICE_SYSTEM_ID MATCHES armv7l)
    set(TOOLCHAIN_NAME arm-linux-gnueabihf)
    add_definitions(-DPLATFORM_ARCH_arm=1)
else ()
    message(FATAL_ERROR "FATAL: Please confirm your platform. SYSTEM_ID:${DEVICE_SYSTEM_ID}")
endif ()

file(GLOB_RECURSE MODULE_COMMON_SRC ../common/*.c)
file(GLOB_RECURSE MODULE_HAL_SRC hal/*.c)
file(GLOB_RECURSE MODULE_APP_SRC application/*.c)
file(GLOB_RECURSE MODULE_SAMPLE_SRC ../../../module_sample/*.c)

include_directories(../../../module_sample)
include_directories(../common)
include_directories(../manifold2/application)

include_directories(../../../../../psdk_lib/include)
link_directories(../../../../../psdk_lib/lib/${TOOLCHAIN_NAME})
link_libraries(${CMAKE_CURRENT_LIST_DIR}/../../../../../psdk_lib/lib/${TOOLCHAIN_NAME}/lib${PACKAGE_NAME}.a)

if (NOT EXECUTABLE_OUTPUT_PATH)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
endif ()

add_executable(${PROJECT_NAME}
        ${MODULE_APP_SRC}
        ${MODULE_SAMPLE_SRC}
        ${MODULE_COMMON_SRC}
        ${MODULE_HAL_SRC})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../common/3rdparty)
find_package(OPUS REQUIRED)
if (OPUS_FOUND)
    message(STATUS "Found OPUS installed in the system")
    message(STATUS " - Includes: ${OPUS_INCLUDE_DIR}")
    message(STATUS " - Libraries: ${OPUS_LIBRARY}")

    add_definitions(-DOPUS_INSTALLED)
    target_link_libraries(${PROJECT_NAME} ${OPUS_LIBRARY})
else ()
    message(STATUS "Cannot Find OPUS")
endif (OPUS_FOUND)


find_package(ALSA REQUIRED)
if (ALSA_FOUND)
    message(STATUS "Found ALSA installed in the system")
    message(STATUS " - Includes: ${ALSA_INCLUDE_DIR}")
    message(STATUS " - Libraries: ${ALSA_LIBRARY}")

    add_definitions(-DALSA_INSTALLED)
    include_directories(${ALSA_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${ALSA_LIBRARIES})
else()
    message(FATAL_ERROR "Cannot Find ALSA")
endif()



find_package(LIBUSB REQUIRED)
if (LIBUSB_FOUND)
    message(STATUS "Found LIBUSB installed in the system")
    message(STATUS " - Includes: ${LIBUSB_INCLUDE_DIR}")
    message(STATUS " - Libraries: ${LIBUSB_LIBRARY}")

    add_definitions(-DLIBUSB_INSTALLED)
    target_link_libraries(${PROJECT_NAME} ${LIBUSB_LIBRARY})
else ()
    message(STATUS "Cannot Find LIBUSB")
endif (LIBUSB_FOUND)



find_package(FFMPEG REQUIRED)
if(FFMPEG_FOUND)
    message(STATUS "Found FFMPEG installed in the system")
    message(STATUS " - Includes: ${FFMPEG_INCLUDE_DIR}")
    message(STATUS " - Libraries: ${FFMPEG_LIBRARIES}")

    ADD_DEFINITIONS(-DFFMPEG_INSTALLED)
    target_link_libraries(${PROJECT_NAME} ${FFMPEG_LIBRARIES} m stdc++)
else ()
    message(FATAL_ERROR "Cannot Find FFMPEG")
endif (FFMPEG_FOUND)



# # 查找 FFmpeg 库
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(AVFORMAT REQUIRED libavformat)
# pkg_check_modules(AVCODEC REQUIRED libavcodec)
# pkg_check_modules(AVUTIL REQUIRED libavutil)

# # 包含 FFmpeg 头文件路径
# include_directories(${AVFORMAT_INCLUDE_DIRS})
# include_directories(${AVCODEC_INCLUDE_DIRS})
# include_directories(${AVUTIL_INCLUDE_DIRS})

# # 链接 FFmpeg 库
# target_link_libraries(${PROJECT_NAME} ${AVFORMAT_LIBRARIES} ${AVCODEC_LIBRARIES} ${AVUTIL_LIBRARIES})


# target_link_libraries(${PROJECT_NAME} m)

# target_link_libraries(${PROJECT_NAME} stdc++)

# add_custom_command(TARGET ${PROJECT_NAME}
#         PRE_LINK COMMAND cmake ..
#         WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

