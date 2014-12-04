##  OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
list(APPEND PTAM_EXTERNAL_LIBS ${OpenCV_LIBRARIES})
message("Found OpenCV libs:${OpenCV_LIBRARIES}")
message("OpenCV INC DIRS: ${OpenCV_INCLUDE_DIRS}")

# OpenGL
find_package(OpenGL REQUIRED)
message(STATUS "Found OpenGL ? ${OPENGL_FOUND}")
if(OPENGL_FOUND)
  message(STATUS "OpenGL INCLUDE: ${OPENGL_INCLUDE_DIR}")
  include_directories(${OPENGL_INCLUDE_DIR})
  list(APPEND PTAM_EXTERNAL_LIBS ${OPENGL_LIBRARIES})
endif()

# GLUT
find_package(GLUT REQUIRED)
if(GLUT_FOUND)
  include_directories(${GLUT_INCLUDE_DIR})
  list(APPEND PTAM_EXTERNAL_LIBS ${GLUT_LIBRARIES})
endif()

if(WIN32)
  # GLEW
  find_package(GLEW REQUIRED)
  if(GLEW_FOUND)
    add_definitions(-DLOOK3D_HAVE_GLEW)
    include_directories(${GLEW_INCLUDE_DIR})
    list(APPEND PTAM_EXTERNAL_LIBS ${GLEW_LIBRARIES})
    set(GLEW_BINARY_DIR "${GLEW_INCLUDE_DIR}/../bin/Release/Win32")
  endif()
endif()