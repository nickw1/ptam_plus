#include "ptam/ui/gl_window.h"
#include <gvars3/GStringUtil.h>
#include <gvars3/instances.h>
#include <TooN/helpers.h>
#include "ptam/ui/gl_helpers.h"
#include "ptam/ui/gl_window_menu.h"

namespace ptam {
  TooN::Vector<2> GLWindow::image_size_;
  int GLWindow::cur_width_;
  int GLWindow::cur_height_;
  std::vector<GLWindowMenu*> GLWindow::gl_menus_;
  GLWindow::Properties GLWindow::properties;
  GLWindowCallback* GLWindow::callback_;
  RenderType* GLWindow::arrender_;
  TooN::Vector<6> GLWindow::mc_pose_update_;
  TooN::Vector<6> GLWindow::left_pose_update_;


  GLWindow::GLWindow(TooN::Vector<2> image_size, GLWindowCallback* callback,
                     std::string window_name) {
    image_size_ = image_size;
    arrender_ = nullptr;
    callback_ = callback;
    callback_->gl_window_ = this;
    Init(window_name.c_str(), image_size[0], image_size[2]);
  }

  GLWindow::GLWindow(RenderType* arrender, GLWindowCallback* callback) {
    arrender_ = arrender;
    callback_ = callback;
    callback_->gl_window_ = this;
    callback_->render_ = arrender;
    image_size_ = arrender->GetImageSize();
  }

  // initialization for the GLWindow object
  void GLWindow::Init(const char* ca_name, int width, int height,
                      int argc, char** argv) {
    properties.window_width = width;
    properties.window_height = height;
    properties.b_full_screen = true;
    properties.window_name = ca_name;

    // OpenGL stuff
    glutInit(&argc, argv);  // using glut for window
    GLInit();               // opengl (& glut) initialization

#ifdef WIN32
    if (GLEW_OK != glewInit()) {
      std::cout << "GLEW init failed";
      exit(1);
    }
#endif

    GVars3::GUI.RegisterCommand("GLWindow.AddMenu", GUICommandCallBack, this);
    //glSetFont("sans");

    //glutFullScreen();
    glutMainLoop();         // for glut

    if (arrender_ != nullptr)
      arrender_->Init();
  }

  void GLWindow::GLInit() {
    // glut window initialisation
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(properties.init_pos_x, properties.init_pos_y);
    glutInitWindowSize(properties.window_width, properties.window_height);
    properties.glut_window_id =
      glutCreateWindow(properties.window_name.c_str());

    // glut callbacks setting
    glutReshapeFunc(Reshape);
    glutIdleFunc(Idle);
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
  }

  void GLWindow::Update() { glutSwapBuffers(); }

  // Render graphics (video frame & necessary objects)
  void GLWindow::Display() {
    callback_->on_display();
    Update();
  }

  // for glut keyboard handling
  void GLWindow::Keyboard(unsigned char ckey, int x, int y) {
    callback_->keyboard(ckey, x, y);
    switch (ckey) {
    case 0x1b:  // ESC
      GLWindow::Release();  // release object
      break;
    case 'w':
      GLWindow::ToggleFullscreen();  // full screen or normal screen
      break;
    case 'p':
      Pause();
      break;
    case 'o':
      if (arrender_)
        arrender_->set_render_mode(RenderType::ORIGINAL);
      break;
    case 'd':
      if (arrender_)
        arrender_->set_render_mode(RenderType::DISTORT);
      break;
    case 'u':
      if (arrender_)
        arrender_->set_render_mode(RenderType::UNDISTORT);
      break;
    }
  }

  void GLWindow::Idle() {
    glutPostRedisplay();
  }

  void GLWindow::Reshape(const int w, const int h) {
    if (arrender_)
      arrender_->resize_window(w, h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Update();
    cur_width_ = w;
    cur_height_ = h;
  }

  void GLWindow::Release() {
    exit(1);
  }

  void GLWindow::ToggleFullscreen() {
    if (properties.b_full_screen) {
      glutPositionWindow(properties.init_pos_x, properties.init_pos_y);
      glutReshapeWindow(properties.window_width, properties.window_height);
      
      if (arrender_)
        arrender_->resize_window(properties.window_width,
                               properties.window_height);
    } else {
      glutFullScreen();
    }
    properties.b_full_screen = !properties.b_full_screen;
  }

  void GLWindow::MouseFunc(int button, int state, int x, int y) {
    bool handled = false;
    for (unsigned int i = 0; !handled && i < gl_menus_.size(); i++)
      handled = gl_menus_[i]->HandleClick(button, state, x, y);

    static bool was_up = false;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
      was_up = true;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      CVD::ImageRef where(x, y);
      static CVD::ImageRef last_mouse_loc;
      if (was_up) {
        last_mouse_loc = where;
      }
      was_up = false;
      CVD::ImageRef motion = where - last_mouse_loc;
      last_mouse_loc = where;

      double sensitivity = 0.01;
      if (state & BUTTON_LEFT && !(state & BUTTON_RIGHT)) {
        mc_pose_update_[3] -= motion[1] * sensitivity;
        mc_pose_update_[4] += motion[0] * sensitivity;
      } else if (!(state & BUTTON_LEFT) && state & BUTTON_RIGHT) {
        left_pose_update_[4] -= motion[0] * sensitivity;
        left_pose_update_[3] += motion[1] * sensitivity;
      } else if (state & BUTTON_MIDDLE || (state & BUTTON_LEFT && state & BUTTON_RIGHT)) {
        left_pose_update_[5] -= motion[0] * sensitivity;
        left_pose_update_[2] += motion[1] * sensitivity;
      }
    }
  }

  void GLWindow::Pause() {
    if (!properties.pausing) {
      glutDisplayFunc(GLWindow::Display);
    } else {
      glutDisplayFunc(GLWindow::Idle);
    }
    properties.pausing = !properties.pausing;
  }

  // Menu interface:
  void GLWindow::AddMenu(std::string sName, std::string sTitle) {
    gl_menus_.push_back(new GLWindowMenu(sName, sTitle));
  }

  void GLWindow::DrawMenus() {
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMask(1, 1, 1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    SetupWindowOrtho();
    glLineWidth(1);

    int nTop = 30;
    int nHeight = 30;
    for (auto& menu : gl_menus_) {
      menu->Render(nTop, nHeight, cur_width_);
      nTop += nHeight + 1;
    }
  }

  // Text display functions:
  void GLWindow::PrintString(CVD::ImageRef irPos, std::string s) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glTranslatef(irPos.x, irPos.y, 0.0);
    glScalef(8, -8, 1);
    //glDrawText(s, NICE, 1.6, 0.1);
    glDrawText(s);
    glPopMatrix();
  }

  void GLWindow::DrawCaption(std::string s) {
    if (s.length() == 0)
      return;

    SetupWindowOrtho();
    // Find out how many lines are in the caption:
    // Count the endls
    int nLines = 0;
    {
      std::string sendl("\n");
      std::string::size_type st = 0;
      while (1) {
        nLines++;
        st = s.find(sendl, st);
        if (st == std::string::npos)
          break;
        else
          st++;
      }
    }

    int nTopOfBox = cur_height_ - nLines * 17;

    // Draw a grey background box for the text
    glColor4f(0, 0, 0, 0.4);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glVertex2d(-0.5, nTopOfBox);
    glVertex2d(cur_width_, nTopOfBox);
    glVertex2d(cur_width_, cur_height_);
    glVertex2d(-0.5, cur_height_);
    glEnd();

    // Draw the caption text in yellow
    glColor3f(1, 1, 0);
    PrintString(CVD::ImageRef(10, nTopOfBox + 13), s);
  }

  void GLWindow::GUICommandHandler(std::string sCommand, std::string sParams) {  // Called by the callback func..
    std::vector<std::string> vs = GVars3::ChopAndUnquoteString(sParams);
    if (sCommand == "GLWindow.AddMenu") {
      switch (vs.size()) {
      case 1:
        AddMenu(vs[0], "Root");
        return;
      case 2:
        AddMenu(vs[0], vs[1]);
        return;
      default:
        std::cout << "? AddMenu: need one or two params (internal menu name, [caption])." << std::endl;
        return;
      };
    };

    // Should have returned to caller by now - if got here, a command which
    // was not handled was registered....
    std::cout << "! GLWindow::GUICommandHandler: unhandled command " << sCommand << std::endl;
    exit(1);
  }

  void GLWindow::GUICommandCallBack(void* ptr, std::string sCommand, std::string sParams) {
    ((GLWindow*)ptr)->GUICommandHandler(sCommand, sParams);
  }

  std::pair<TooN::Vector<6>, TooN::Vector<6> > GLWindow::GetMousePoseUpdate() {
    auto result = std::make_pair(left_pose_update_, mc_pose_update_);
    left_pose_update_ = TooN::Zeros;
    mc_pose_update_ = TooN::Zeros;
    return result;
  }

  void GLWindow::SetupWindowOrtho() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, cur_width_, cur_height_, 0, -1, 1);
  }

  void GLWindow::SetupViewport() {
    glViewport(0, 0, cur_width_, cur_height_);
  }

  void GLWindow::SetupVideoOrtho() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    ::glOrtho(-0.5, (double)image_size_[0] - 0.5,
             (double)image_size_[1] - 0.5, -0.5, -1.0, 1.0);
  }

  void GLWindow::SetupVideoRasterPosAndZoom() {

  }
}  // namespace ptam