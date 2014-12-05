// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
//
// MapViewer.h
//
// Defines the MapViewer class
//
// This defines a simple map viewer widget, which can draw the
// current map and the camera/keyframe poses within it.
//
#pragma once

#include "ptam/construct/map.h"
#include <TooN/TooN.h>
using namespace TooN;
#include <TooN/se3.h>
#include <sstream>
#include "ptam/ui/gl_window.h"

namespace ptam {
class MapViewer {
 public:
  MapViewer(Map &map, GLWindow &glw);
  void DrawMap(TooN::SE3<> se3CamFromWorld);
  std::string GetMessageForUser();

 protected:
  Map &mMap;
  GLWindow &mGLWindow;

  void DrawGrid();
  void DrawMapDots();
  void DrawCamera(TooN::SE3<> se3, bool bSmall=false);
  void SetupFrustum();
  void SetupModelView(TooN::SE3<> se3WorldFromCurrent = TooN::SE3<>());

  TooN::Vector<3> mv3MassCenter;
  TooN::SE3<> mse3ViewerFromWorld;

  std::ostringstream mMessageForUser;
};
}  // namespace ptam