cmake_minimum_required(VERSION 2.8)
project(ptam_test)

##==============================================================================
##  executables
if(WITH_UI)
ptam_add_executable(test_ui_ar_render test_ui_ar_render.cc)
endif()


