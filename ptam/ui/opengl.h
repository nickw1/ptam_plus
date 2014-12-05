// Copyright 2008 Isis Innovation Limited
#pragma once
#include <string>

// NOTE: OpenGL combined with Eigen needs to be initialized in this order.
// if you change the order, you might get linker errors!

#if defined(__APPLE__)  // MaC platform ========================================
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
  #include <GLUT/glut.h>

#elif defined(WIN32)  // WINDOWS platform ======================================

  #include <Windows.h>
  // use GLEW => less painful
  #include <GL/glew.h>  // this has to be before GL headers
  #include <GL/wglew.h>

  #include <GL/gl.h>
  #include <GL/glu.h>

  #include <GL/glut.h>
  #include <GL/freeglut_ext.h>

#elif defined(ANDROID)  // ANDROID platform ====================================
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <GLES2/gl2platform.h>
  #define FREEGLUT_GLES
  #include <GL/glut.h>
#elif defined(__linux)  // linux
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <GL/glut.h>
#else
    #error Unsupported platform
#endif

inline bool glError(const char *file, int line) {
  GLenum err (glGetError());
  int count = 0;
  while(err != GL_NO_ERROR) {
    std::string error;

    switch(err) {
    case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
    case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
    case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
    case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
//    case GL_INVALID_FRAMEBUFFER_OPERATION:
//      error="INVALID_FRAMEBUFFER_OPERATION";
//      break;
    }

    printf("GL_%s - %s:%d", error.c_str(), file, line);
    err = glGetError();
  }
  return count;
}

#ifdef _DEBUG
#define glCheckError() glError(__FILE__,__LINE__)
#define glCountError() glError(__FILE__,__LINE__)
#else
#define glCheckError()
#define glCountError() 0
#endif