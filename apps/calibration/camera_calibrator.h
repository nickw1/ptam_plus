// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
#pragma once
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <gvars3/gvars3.h>
#include "ptam/ui/gl_window.h"
#include "calib_image.h"

namespace ptam {
class CameraCalibrator : public GLWindowCallback {
public:
  CameraCalibrator();

  void Reset();
  void HandleFrame(CVD::Image<CVD::byte> imFrame);
  static void MainLoopCallback(void* pvUserData);
  void MainLoopStep();
  
  cv::VideoCapture capture;

  std::shared_ptr<GLWindow> mGLWindow;
  ATANCamera mCamera;
  bool mbDone;

  std::vector<CalibImage> mvCalibImgs;
  void OptimizeOneStep();

  bool mbGrabNextFrame;
  GVars3::gvar3<int> mgvnOptimizing;
  GVars3::gvar3<int> mgvnShowImage;
  GVars3::gvar3<int> mgvnDisableDistortion;
  double mdMeanPixelError;

  void GUICommandHandler(std::string sCommand, std::string sParams);
  static void GUICommandCallBack(void* ptr, std::string sCommand, std::string sParams);

  virtual void on_display();
};
}  // namespace ptam