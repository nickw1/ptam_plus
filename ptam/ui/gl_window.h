// Copyright(C) 2007-2014 The PTAM Authors. All rights reserved.
#pragma once

#include <string>
#include <vector>
#ifdef WIN32
#include <windows.h>
#endif
#include <cvd/image.h>
#include "ptam/ui/opengl.h"
#include "ptam/ui/ar_render.h"

namespace ptam {
  class ATANCamera;
  class GLWindowMenu;
  class GLWindow;
  typedef ARRender<ATANCamera>  RenderType;

  class GLWindowCallback {
  public:
    GLWindowCallback() { render_ = nullptr; }
    virtual ~GLWindowCallback() {}
    virtual void on_display() {}
    virtual void keyboard(unsigned char c_key, int x, int y) {}

  protected:
    friend class GLWindow;
    GLWindow* gl_window_;
    RenderType* render_;
  };

  class GLWindow {
  public:
    /// Symbols for mouse buttons and modifiers
    enum MouseButton {
      BUTTON_LEFT = GLUT_LEFT_BUTTON,
      BUTTON_MIDDLE = GLUT_MIDDLE_BUTTON,
      BUTTON_RIGHT = GLUT_RIGHT_BUTTON,
      BUTTON_MOD_CTRL = 8,
      BUTTON_MOD_SHIFT = 0x10,
      BUTTON_WHEEL_UP = 0x20,
      BUTTON_WHEEL_DOWN = 0x40
    };

    struct Properties {
      Properties() {
        glut_window_id = 0;
        window_width = 640;
        window_height = 480;
        init_pos_x = 100;
        init_pos_y = 100;
        window_name = "Put Window's Name Here";
        b_full_screen = false;  // fullscreen or not
        pausing = false;  // pause or not
      }
      int glut_window_id;
      int window_width, window_height, init_pos_x, init_pos_y;
      std::string window_name;

      bool b_full_screen;  // fullscreen or not
      bool pausing;  // pause or not
    };

    GLWindow() = delete;
    GLWindow(const GLWindow&) = delete;
    GLWindow(TooN::Vector<2> image_size, GLWindowCallback* callback,
             std::string window_name);
    GLWindow(RenderType* arrender, GLWindowCallback* callback);

    // initialization for the GLWindow object
    void Init(const char* ca_name, int width, int height,
              int argc = 1, char** argv = NULL);
    void GLInit();

    // GLUT callbacks ============================================================
    static void Update();

    // Render graphics (video frame & necessary objects)
    static void Display();

    // for glut keyboard handling
    static void Keyboard(unsigned char ckey, int x, int y);

    static void Idle();

    static void Reshape(const int w, const int h);

    static void Release();

    static void ToggleFullscreen();

    static void MouseFunc(int button, int state, int x, int y);

    static void Pause();

    // Menu interface ============================================================
    static void AddMenu(std::string sName, std::string sTitle);
    static void DrawMenus();

    // Text display functions ====================================================
    static void PrintString(CVD::ImageRef irPos, std::string s);
    static void DrawCaption(std::string s);

    // Map viewer mouse interface:
    static std::pair<TooN::Vector<6>, TooN::Vector<6> > GetMousePoseUpdate();

    static void SetupWindowOrtho();
    static void SetupViewport();
    static void SetupVideoOrtho();
    static void SetupVideoRasterPosAndZoom();

    static int width() { return cur_width_; }
    static int height() { return cur_height_; }

  protected:
    static void GUICommandHandler(std::string sCommand, std::string sParams);
    static void GUICommandCallBack(void* ptr, std::string sCommand, std::string sParams);

    static TooN::Vector<2> image_size_;
    static int cur_width_, cur_height_;

    // User interface menus:
    static std::vector<GLWindowMenu*> gl_menus_;

    static Properties properties;
    static GLWindowCallback* callback_;
    static RenderType* arrender_;

    // Storage for map viewer updates:
    static TooN::Vector<6> mc_pose_update_;
    static TooN::Vector<6> left_pose_update_;
  };
}  // namespace ptam