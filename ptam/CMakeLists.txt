cmake_minimum_required(VERSION 2.8)

project(ptam_library)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(sub_dirs construct math track util)

if(PTAM_WITH_UI)
  list(APPEND sub_dirs ui)
endif(PTAM_WITH_UI)

set(ptam_sources "")
foreach(sub_dir ${sub_dirs})
  file(GLOB ${sub_dir}_sources  ${CMAKE_CURRENT_SOURCE_DIR}/${sub_dir}/*.cc
                                ${CMAKE_CURRENT_SOURCE_DIR}/${sub_dir}/*.h)
  list(APPEND ptam_sources ${${sub_dir}_sources})
endforeach()

if(NOT PTAM_WITH_UI)
  set(remove_items "track/calib_corner_patch*"
                   "track/calib_image*")
    foreach(item ${remove_items})
      file(GLOB temp_src ${CMAKE_CURRENT_SOURCE_DIR}/${item})
      foreach(temp_item ${temp_src})
        list(REMOVE_ITEM ptam_sources ${temp_item})
      endforeach()
    endforeach()
endif()

add_library(PTAM ${PTAM_LIB_TYPE} ${ptam_sources})
target_link_libraries(PTAM ${PTAM_EXTERNAL_LIBS})
set_property(TARGET PTAM PROPERTY FOLDER "ptam/libs")


# ==============================================================================
# Optional Configuration
option(PTAM_WITH_TEST "Build Library with Tests" OFF)

if(PTAM_WITH_TEST)
  #enable_testing() # has to be in root CMakeLists.txt for target 'test' to work!
  add_subdirectory(test)
endif(PTAM_WITH_TEST)
