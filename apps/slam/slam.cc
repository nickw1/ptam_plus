#include <stdlib.h>
#include <iostream>
#include <gvars3/instances.h>
#include "slam_system.h"
#include "config.h"

using namespace ptam;
using namespace i3d;

int main(int argc, char **argv) {
  printf("PTAM \n");
  printf("  --------------- \n");
  printf("  Parsing settings.cfg ....\n");
  std::string cam_configfile = (argc > 1) ? argv[1] : CAMERA_CONFIG_FILE;
  std::string configfile = (argc > 2) ? argv[2] : CONFIG_FILE;
  GVars3::GUI.LoadFile(cam_configfile);
  GVars3::GUI.LoadFile(configfile);

  // Parsing console input
  GVars3::GUI.StartParserThread();
  atexit(GVars3::GUI.StopParserThread);

  try {
    SlamSystem sys;
    sys.Run();
  } catch (CVD::Exceptions::All e) {
    printf("Oops! slam.cc main()::%s\n", e.what.c_str());
  }
}










