cmake_minimum_required(VERSION 2.8.9)

set(THE_PROJECT_NAME PTAM_and_apps)
project(${THE_PROJECT_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # for organizing code to folders

set(PTAM_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT THE_PROJECT_ROOT OR THE_PROJECT_ROOT STREQUAL "")
  set(THE_PROJECT_ROOT ${PTAM_ROOT})
endif()

## Add directories of own CMake modules
# e.g. define ICG_CMAKE_MODULE_PATH in your environment
file(TO_CMAKE_PATH "$ENV{CMAKE_MODULE_PATH}" OT_CMAKE_MODULE_PATH)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
                      ${CMAKE_CURRENT_SOURCE_DIR}/cmake
                      ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )
include(PTAMUtil)
message("PTAM cmake modules: " ${CMAKE_MODULE_PATH} )

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

list(APPEND THE_DEPEDENCIES_BINARY_PATHS ${THE_LIB_RUNTIME_OUTPUT_DIRECTORY})

set(PTAM_WORLD_VERSION 1)
set(PTAM_MAJOR_VERSION 0)
set(PTAM_MINOR_VERSION 0)
message(STATUS "Welcome! ptam version ${PTAM_COLOR_BLUE}${PTAM_WORLD_VERSION}.${PTAM_MAJOR_VERSION}.${PTAM_MINOR_VERSION}${PTAM_COLOR_RESET}")
message(STATUS "ptam cmake module paths: ${PTAM_COLOR_BLUE}${CMAKE_MODULE_PATH}${PTAM_COLOR_RESET}")

set(PTAM_CXX_FLAGS)
add_definitions(-DPTAM_ROOT="${PTAM_ROOT}")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(PTAM_CXX_FLAGS " -stdlib=libc++ ")  # prevent default libc++ on new mac
endif()

## Common configuration
add_definitions(-Dptam3_HAVE_TOON -DTOON_TYPEOF_DECLTYPE)
add_compile_options("-pthread")
add_link_options("-pthread")
add_compile_options("-g")
add_compile_options("-fsanitize=address")
set(CMAKE_CXX_FLAGS ${PTAM_CXX_FLAGS})
PTAM_build_setting()

## Build Options ===============================================================
# Allow the developer to select if Dynamic or Static libraries are built
option(PTAM_BUILD_SHARED_LIBS "Build Shared Libraries (preferred and required for the g2o plugin system)" OFF)
set(PTAM_LIB_TYPE STATIC)
if(PTAM_BUILD_SHARED_LIBS)
  set(PTAM_LIB_TYPE SHARED)
  add_definitions(-DPTAM_SHARED_LIBS)
endif()

## External libraries (included in this repo) ==================================
# F2C, BLAS, CLAPACK
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/clapack-3.2.1)
  set(CLAPACK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/clapack-3.2.1/INCLUDE)
  add_subdirectory(external/clapack-3.2.1)
  include_directories(${CLAPACK_INCLUDE_DIR})
  list(APPEND PTAM_EXTERNAL_LIBS f2c blas lapack)

  set_property(TARGET f2c PROPERTY FOLDER "external")
  set_property(TARGET blas PROPERTY FOLDER "external")
  set_property(TARGET lapack PROPERTY FOLDER "external")
else()
  find_package(BLAS REQUIRED)
  find_package(LAPACK REQUIRED)
  list(APPEND PTAM_EXTERNAL_LIBS ${F2C_LIBRARIES} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
endif()

# TooN
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/TooN)
  set(TooN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
elseif(NOT TooN_INCLUDE_DIR OR TooN_INCLUDE_DIR STREQUAL "")
  message(ERROR "Please set TooN_INCLUDE_DIR to the TooN library folder")
endif()
include_directories(${TooN_INCLUDE_DIR})

##  LibCVD
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/libcvd)
  add_subdirectory(external/libcvd)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/libcvd)
  list(APPEND PTAM_EXTERNAL_LIBS libcvd)
  set_property(TARGET libcvd PROPERTY FOLDER "external")
else()
  find_package(CVD REQUIRED)
  include(${CVD_INCLUDE_DIR})
  list(APPEND PTAM_EXTERNAL_LIBS ${CVD_LIBRARY})
endif()

## GVars3
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/gvars-3.0)
  add_subdirectory(external/gvars-3.0)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/gvars-3.0)
  list(APPEND PTAM_EXTERNAL_LIBS gvars)
  set_property(TARGET gvars PROPERTY FOLDER "external")
else()
  find_package(GVars3 REQUIRED)
  include(${GVars3_INCLUDE_DIR})
  list(APPEND PTAM_EXTERNAL_LIBS ${GVars3_LIBRARY})
endif()

option(PTAM_WITH_UI "Build Library with UI" OFF)
if(PTAM_WITH_UI)
  include(PTAM_UI_Dependencies)  # more dependencies: opencv, opengl, glut, glew.
endif()

## ptam library ===============================================================
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(ptam)
set(PTAM_LIBS PTAM)

# applications
add_subdirectory(apps)

# Useful paths configuration for windows =======================================
if(WIN32)
  if(MSVC)
    if(CMAKE_CL_64)
      set(LsdSlam_ARCH x64)
    else()
      set(LsdSlam_ARCH x86)
    endif()
    if(MSVC_VERSION EQUAL 1400)
      set(LsdSlam_RUNTIME vc8)
    elseif(MSVC_VERSION EQUAL 1500)
      set(LsdSlam_RUNTIME vc9)
    elseif(MSVC_VERSION EQUAL 1600)
      set(LsdSlam_RUNTIME vc10)
      get_filename_component(VC_IDE_PATH $ENV{VS100COMNTOOLS}/../IDE ABSOLUTE)
    elseif(MSVC_VERSION EQUAL 1700)
      set(LsdSlam_RUNTIME vc11)
      get_filename_component(VC_IDE_PATH $ENV{VS110COMNTOOLS}/../IDE ABSOLUTE)
    elseif(MSVC_VERSION EQUAL 1800)
      set(LsdSlam_RUNTIME vc12)
      get_filename_component(VC_IDE_PATH $ENV{VS120COMNTOOLS}/../IDE ABSOLUTE)
    endif()
  endif()
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/set_paths.bat.in" 
                 "${CMAKE_CURRENT_BINARY_DIR}/set_paths.bat")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/set_paths.bat.in" 
                 "${THE_LIB_RUNTIME_OUTPUT_DIRECTORY}/set_paths.bat")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/set_paths_and_run_vc.bat.in"
                 "${CMAKE_CURRENT_BINARY_DIR}/set_paths_and_run_vc.bat")
endif()
