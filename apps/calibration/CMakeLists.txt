cmake_minimum_required(VERSION 2.8)
project(calibration)

## application specific
ptam_build_setting()

##==============================================================================
# This is the path where the images and testdata are stored
set( CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR})
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config.h )

##==============================================================================
set(ALL_LIBRARIES ${PTAM_LIBS} ${PTAM_EXTERNAL_LIBS})
ptam_add_executable(calibrator camera_calibrator.cc)
set_property(TARGET calibrator PROPERTY FOLDER "ptam/apps")
