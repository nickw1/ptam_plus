# Install script for directory: C:/Users/Thanh/dev/ptam_plus/external/clapack-3.2.1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/CLAPACK")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Thanh/dev/ptam_plus/external/clapack-3.2.1/build-x86-vc12/F2CLIBS/cmake_install.cmake")
  include("C:/Users/Thanh/dev/ptam_plus/external/clapack-3.2.1/build-x86-vc12/BLAS/cmake_install.cmake")
  include("C:/Users/Thanh/dev/ptam_plus/external/clapack-3.2.1/build-x86-vc12/SRC/cmake_install.cmake")
  include("C:/Users/Thanh/dev/ptam_plus/external/clapack-3.2.1/build-x86-vc12/Testing/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "C:/Users/Thanh/dev/ptam_plus/external/clapack-3.2.1/build-x86-vc12/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "C:/Users/Thanh/dev/ptam_plus/external/clapack-3.2.1/build-x86-vc12/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
