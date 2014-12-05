// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
//
// SmallBlurryImage-based relocaliser
//
// Each KF stores a small, blurred version of itself;
// Just compare a small, blurred version of the input frame to all the KFs,
// choose the closest match, and then estimate a camera rotation by direct image
// minimisation.
#pragma once
#include <TooN/se2.h>
#include "ptam/math/atan_camera.h"
#include "ptam/construct/small_blurry_image.h"
#include "ptam/construct/map.h"

namespace ptam {
class Relocaliser {
 public:
  Relocaliser(Map &map, ATANCamera &camera);
  bool AttemptRecovery(KeyFrame &k);
  TooN::SE3<> BestPose();

 protected:
  void ScoreKFs(KeyFrame &kCurrentF);
  Map &map_;
  ATANCamera camera_;
  int nbest_;
  double best_score_;
  TooN::SE2<> se2_pose_;
  TooN::SE3<> se3_best_pose_;

};
}  // namespace ptam