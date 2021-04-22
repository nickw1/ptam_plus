cmake_minimum_required(VERSION 2.8)
project(slam)

## application specific
ptam_build_setting()

##==============================================================================
# This is the path where the images and testdata are stored
set( CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR})
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config.h )

##==============================================================================
set(ALL_LIBRARIES ${PTAM_LIBS} ${PTAM_EXTERNAL_LIBS})
#ptam_add_executable(slam slam.cc slam_system.cc slam_system.h)
#set_property(TARGET slam PROPERTY FOLDER "ptam/apps")

ptam_add_executable(slam_glut slam_glut.cc slam_window_glut.h)
set_property(TARGET slam_glut PROPERTY FOLDER "ptam/apps")