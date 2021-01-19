cmake_minimum_required(VERSION 2.8.9)

set(THE_PROJECT_NAME GVars)
project(${THE_PROJECT_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # for organizing code to folders

set(GVars_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT THE_PROJECT_ROOT OR THE_PROJECT_ROOT STREQUAL "")
  set(THE_PROJECT_ROOT ${GVars_ROOT})
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

set(GVars_WORLD_VERSION 1)
set(GVars_MAJOR_VERSION 0)
set(GVars_MINOR_VERSION 0)
message(STATUS "Welcome! GVars version ${GVars_COLOR_BLUE}${GVars_WORLD_VERSION}.${GVars_MAJOR_VERSION}.${GVars_MINOR_VERSION}${GVars_COLOR_RESET}")
message(STATUS "GVars cmake module paths: ${GVars_COLOR_BLUE}${CMAKE_MODULE_PATH}${GVars_COLOR_RESET}")

set(GVars_CXX_FLAGS)
add_definitions(-DGVars_ROOT="${GVars_ROOT}")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(GVars_CXX_FLAGS " -stdlib=libc++ ")  # prevent default libc++ on new mac
endif()

## Common configuration
add_definitions(-DGVARS3_HAVE_TOON -DTOON_TYPEOF_DECLTYPE)


## Build Options ===============================================================
set(CMAKE_CXX_FLAGS ${GVars_CXX_FLAGS})
#cmaker_common_build_setting()

# Allow the developer to select if Dynamic or Static libraries are built
option(GVars_BUILD_SHARED_LIBS "Build Shared Libraries (preferred and required for the g2o plugin system)" OFF)
set(GVars_LIB_TYPE STATIC)
if(GVars_BUILD_SHARED_LIBS)
  set(GVars_LIB_TYPE SHARED)
  add_definitions(-DGVars_SHARED_LIBS)
endif()

## External libraries (included in this repo) ==================================
# TooN
if (NOT TooN_INCLUDE_DIR OR TooN_INCLUDE_DIR STREQUAL "")
  set(TooN_INCLUDE_DIR "")
  message(ERROR "Please set TooN_INCLUDE_DIR to the TooN library folder")
endif()
include_directories(${TooN_INCLUDE_DIR})

## GVars library ===============================================================
set(GVars_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GVars_INCLUDE_DIRS})

file(GLOB gvars_headers   ${CMAKE_CURRENT_SOURCE_DIR}/gvars3/*.h)
file(GLOB gvars_sources   ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
set(gvars_sources ${gvars_sources} ${gvars_headers})

# removing unwanted sources (e.x.: threading, video, etc)
set(remove_items "gvars3/GUI_Fltk*" "src/GUI_Fltk*"
                 "gvars3/GUI_*readline*" "src/GUI_*readline*"
                 "gvars3/GUI_Motif.h" "src/GUI_Motif.cc"
                 "src/GUI_language.cc" "src/inst_headless.cc")
foreach(item ${remove_items})
  file(GLOB temp_src ${CMAKE_CURRENT_SOURCE_DIR}/${item})
  foreach(temp_item ${temp_src})
    list(REMOVE_ITEM gvars_sources ${temp_item})
  endforeach()
endforeach()

add_library(gvars ${GVars_LIB_TYPE} ${gvars_sources})