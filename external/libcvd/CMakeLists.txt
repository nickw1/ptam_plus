cmake_minimum_required(VERSION 2.8.9)

set(THE_PROJECT_NAME LibCVD)
project(${THE_PROJECT_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # for organizing code to folders

set(LibCVD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT THE_PROJECT_ROOT OR THE_PROJECT_ROOT STREQUAL "")
  set(THE_PROJECT_ROOT ${LibCVD_ROOT})
endif()

# postfix, based on type
set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "postfix applied to debug build of libraries")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "postfix applied to release build of libraries")
set(CMAKE_RELWITHDEBINFO_POSTFIX "_rd" CACHE STRING "postfix applied to release-with-debug-information libraries")
set(CMAKE_MINSIZEREL_POSTFIX "_s" CACHE STRING "postfix applied to minimium-size-build libraries")

# work out the postfix; required where we use OUTPUT_NAME
if(CMAKE_BUILD_TYPE MATCHES Release)
  set(EXE_POSTFIX)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
  set(EXE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  set(EXE_POSTFIX ${CMAKE_RELWITHDEBINFO_POSTFIX})
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
  set(EXE_POSTFIX ${CMAKE_MINSIZEREL_POSTFIX})
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(NOT THE_PROJECT_ROOT OR THE_PROJECT_ROOT STREQUAL "")
  cmaker_print_error("Please set THE_PROJECT_ROOT to your project root!!!")
endif()

if(NOT THE_LIB_RUNTIME_OUTPUT_DIRECTORY)
  set(THE_LIB_RUNTIME_OUTPUT_DIRECTORY ${THE_PROJECT_ROOT}/bin CACHE PATH "Target for the binaries")
  set(THE_LIB_LIBRARY_OUTPUT_DIRECTORY ${THE_PROJECT_ROOT}/lib CACHE PATH "Target for the libraries")
endif()
link_directories(${THE_LIB_LIBRARY_OUTPUT_DIRECTORY})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${THE_LIB_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${THE_LIB_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${THE_LIB_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${THE_LIB_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${THE_LIB_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${THE_LIB_RUNTIME_OUTPUT_DIRECTORY})

set(LibCVD_WORLD_VERSION 1)
set(LibCVD_MAJOR_VERSION 0)
set(LibCVD_MINOR_VERSION 0)
message(STATUS "Welcome! LibCVD version ${LibCVD_COLOR_BLUE}${LibCVD_WORLD_VERSION}.${LibCVD_MAJOR_VERSION}.${LibCVD_MINOR_VERSION}${LibCVD_COLOR_RESET}")
message(STATUS "LibCVD cmake module paths: ${LibCVD_COLOR_BLUE}${CMAKE_MODULE_PATH}${LibCVD_COLOR_RESET}")

set(LibCVD_CXX_FLAGS)
add_definitions(-DLibCVD_ROOT="${LibCVD_ROOT}")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(LibCVD_CXX_FLAGS " -stdlib=libstdc++ ")  # prevent default libc++ on new mac
endif()

## Common configuration
add_definitions(-DTOON_TYPEOF_DECLTYPE)


## Build Options ===============================================================
set(CMAKE_CXX_FLAGS ${LibCVD_CXX_FLAGS})
#cmaker_common_build_setting()

# Allow the developer to select if Dynamic or Static libraries are built
option(LibCVD_BUILD_SHARED_LIBS "Build Shared Libraries (preferred and required for the g2o plugin system)" OFF)
set(LibCVD_LIB_TYPE STATIC)
if(LibCVD_BUILD_SHARED_LIBS)
  set(LibCVD_LIB_TYPE SHARED)
  add_definitions(-DLibCVD_SHARED_LIBS)
endif()

## External libraries (included in this repo) ==================================
# TooN
if (NOT TooN_INCLUDE_DIR OR TooN_INCLUDE_DIR STREQUAL "")
  set(TooN_INCLUDE_DIR "")
  message(ERROR "Please set TooN_INCLUDE_DIR to the TooN library folder")
endif()
include_directories(${TooN_INCLUDE_DIR})

## LibCVD library ===============================================================
set(LibCVD_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${LibCVD_INCLUDE_DIRS})

file(GLOB cvd_headers   ${CMAKE_CURRENT_SOURCE_DIR}/cvd/*.h
                        ${CMAKE_CURRENT_SOURCE_DIR}/cvd/internal/*.h
                        ${CMAKE_CURRENT_SOURCE_DIR}/cvd/internal/io/*.h)

set(sub_dirs fast i686)
if(WIN32)
  list(APPEND sub_dirs SSE2 Win32)
elseif(ANDROID)
  lsit(APPEND NEON)
elseif(APPLE)
  list(APPEND sub_dirs SSE2)
else()
  list(APPEND noarch)
endif()

file(GLOB cvd_sources   ${CMAKE_CURRENT_SOURCE_DIR}/cvd_src/*.c*
                        ${CMAKE_CURRENT_SOURCE_DIR}/cvd_src/*.c*)
foreach(sub_dir ${sub_dirs})
  file(GLOB ${sub_dir}_sources  ${CMAKE_CURRENT_SOURCE_DIR}/cvd_src/${sub_dir}/*.c*)
  list(APPEND cvd_sources ${${sub_dir}_sources})
endforeach()

# removing unwanted sources (e.x.: threading, video, etc)
set(remove_items 
                "cvd/eventobject.h" "cvd_src/eventobject.cpp" 
                 "cvd_src/globlist.cxx"
                 "cvd_src/gltext.cpp"
                 "cvd/glwindow.h"    "cvd_src/glwindow.cc"
                 "cvd/thread.h" "cvd_src/thread.cpp"
                 "cvd/synchronized.h" "cvd_src/synchronized.cpp"
                 "cvd/video*" "cvd_src/video*"
                 "cvd_src/fast/fast_7*"
                 "cvd_src/fast/fast_8*"
                 "cvd_src/fast/fast_9*"
                 "cvd_src/fast/fast_11*"
                 "cvd_src/fast/fast_12*"
                 "cvd_src/i686/*MMX*"
                 )
foreach(item ${remove_items})
  file(GLOB temp_src ${CMAKE_CURRENT_SOURCE_DIR}/${item})
  foreach(temp_item ${temp_src})
    list(REMOVE_ITEM cvd_sources ${temp_item})
  endforeach()
endforeach()

add_library(libcvd ${LibCVD_LIB_TYPE} ${cvd_sources} ${cvd_headers})