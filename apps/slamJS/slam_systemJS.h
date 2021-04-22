// Copyright 2008 Isis Innovation Limited
// This stores the main functional classes of the system, like the
// mapmaker, map, tracker etc, and spawns the working threads.
#ifndef PTAM_TRACK_SYSTEM_H_
#define PTAM_TRACK_SYSTEM_H_

#include <cvd/image.h>
#include <cvd/rgb.h>
#include <cvd/byte.h>

#include "construct/map.h"
#include "construct/map_maker.h"
#include "track/tracker.h"

namespace ptam
{

  class SlamSystemJS
  {
  public:
    SlamSystemJS();
    void Run();

    CVD::Image<CVD::Rgb<CVD::byte>> mimFrameRGB;
    CVD::Image<CVD::byte> mimFrameBW;

    Map *mpMap;
    MapMaker *mpMapMaker;
    Tracker *mpTracker;

    bool mbDone;
  };
} // namespace ptam
#endif
