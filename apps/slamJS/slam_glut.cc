#include <iostream>
#include <gvars3/instances.h>

#include "ptam/ui/opengl.h"
#include "ptam/ui/gl_helpers.h"
#include "ptam/ui/ar_render.h"
#include "ptam/ui/gl_window.h"
#include "config.h"
#include "slam_window_glut.h"

using namespace std;
using namespace cv;
using namespace ptam;

static ARRender<ATANCamera>* p_ar_render;
class ARDrawable : public ptam::AbstractDrawable {
public:
  ARDrawable() {}
  void Draw() {
    if (slam_window->map_->IsGood()) {
      glDrawAugmentation(slam_window->tracker_->GetCurrentPose());
    }
  }

  void Draw2D() {
    if (slam_window->map_->IsGood()) {
      glRenderGrid(slam_window->tracker_->GetCurrentPose(), *slam_window->camera_);
      //      glDrawTrackedPoints();
    } else {
      glDrawTrails(slam_window->tracker_->GetTrails());
    }
  }

  SlamWindowCallback<ATANCamera>* slam_window;
};

// sample program for slam  tracking
int main(int argc, char * argv[]) {
  printf("PTAM \n");
  printf("===========================\n");
  printf("Parsing settings.cfg ....\n");
  std::string cam_configfile = (argc > 1) ? argv[1] : CAMERA_CONFIG_FILE;
  std::string configfile = (argc > 2) ? argv[2] : CONFIG_FILE;
  GVars3::GUI.LoadFile(cam_configfile);
  GVars3::GUI.LoadFile(configfile);

//    // Parsing console input
//    GVars3::GUI.StartParserThread();  // thread doesn't work on windows yet (require pthread)
//    atexit(GVars3::GUI.StopParserThread);

  ARDrawable drawable;

  SlamWindowCallback<ATANCamera> slamwindow_callback;

  drawable.slam_window = &slamwindow_callback;

  ARRender<ATANCamera> ar_render;
  ar_render.Configure(slamwindow_callback.camera_.get());
  ar_render.add_drawable(&drawable);
  ar_render.set_render_mode(ar_render.DISTORT);

  p_ar_render = &ar_render;

  GLWindow gl_window(&ar_render, &slamwindow_callback);
  gl_window.Init("Slam Demo", 640, 480, argc, argv);
//    gl_window.ToggleFullscreen();

  return 0;
}

