#pragma once
#include <iostream>
#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ptam/construct/map.h"
#include "ptam/construct/map_maker.h"
#include "ptam/track/tracker.h"
#include "ptam/ui/ar_render.h"
#include "ptam/ui/map_viewer.h"
#include "ptam/ui/gl_window.h"
#include "ptam/util/opencv.h"

namespace ptam
{

  template <typename CameraType>
  class SlamWindowCallback : public ptam::GLWindowCallback
  {
  public:
    SlamWindowCallback()
    {

      if (!capture.open(0))
      {
        printf("ERROR: Cannot open cv::VideoCapture\n");
        exit(1);
      }
      capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
      capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

      // First, check if the camera is calibrated.
      // If not, we need to run the calibration widget.
      TooN::Vector<NUMTRACKERCAMPARAMETERS> vTest;

      vTest = GVars3::GV3::get<TooN::Vector<NUMTRACKERCAMPARAMETERS> >(
          "Camera.Parameters", ATANCamera::mvDefaultParams, GVars3::HIDDEN);
      camera_.reset(new ATANCamera("Camera"));
      if (vTest == ATANCamera::mvDefaultParams)
      {
        printf("! Camera.Parameters is not set, need to run the CameraCalibrator tool\n");
        printf("  and/or put the Camera.Parameters= line into the appropriate .cfg file.\n");
        exit(1);
      }

      TooN::Vector<2> v2 = camera_->GetImageSize();
      CVD::ImageRef img_size(v2[0], v2[1]);
      mimFrameBW.resize(img_size);
      mimFrameRGB.resize(img_size);

      map_.reset(new Map);
      map_maker_.reset(new MapMaker(*map_, *camera_));
      tracker_.reset(new Tracker(img_size, *camera_, *map_, *map_maker_));

      map_viewer_.reset(new MapViewer(*map_, *gl_window_));

      // setup menus
      GVars3::GUI.ParseLine("GLWindow.AddMenu Menu Menu");
      GVars3::GUI.ParseLine("Menu.ShowMenu Root");
      GVars3::GUI.ParseLine("Menu.AddMenuButton Root Reset Reset Root");
      GVars3::GUI.ParseLine("Menu.AddMenuButton Root Spacebar PokeTracker Root");
      GVars3::GUI.ParseLine("DrawAR=0");
      GVars3::GUI.ParseLine("DrawMap=0");
      GVars3::GUI.ParseLine("Menu.AddMenuToggle Root \"View Map\" DrawMap Root");
      GVars3::GUI.ParseLine("Menu.AddMenuToggle Root \"Draw AR\" DrawAR Root");
    }

    cv::VideoCapture capture;

  protected:
    virtual void on_display()
    {
      static cv::Mat clone_rgb;
      try
      {
        /*timeval tim;
      gettimeofday(&tim, NULL);
      double start = tim.tv_sec + (tim.tv_usec / 1000000.0);*/

        // We use two versions of each video frame:
        // One black and white (for processing by the tracker etc)
        // and one RGB, for drawing.

        // Grab new video frame...
        cv::Mat rgb_frame;
        capture.read(rgb_frame);
        // convert video frame to RGB
        cv::cvtColor(rgb_frame, rgb_frame, CV_BGR2RGB);

        if (rgb_frame.cols != 640 || rgb_frame.rows != 480)
          cv::resize(rgb_frame, rgb_frame, cv::Size(640, 480));

        clone_rgb = rgb_frame.clone();

        // In javascript we can maybe just aquire the video frame convert it to gray scale and call cvd_gray_frame
        // and follow the code from there. The color frame we don't actually need IF it is only used for drawing the background

        CVD::SubImage<CVD::Rgb<CVD::byte> > cvd_rgb_frame(
            (CVD::Rgb<CVD::byte> *)rgb_frame.data,
            CVD::ImageRef(rgb_frame.cols, rgb_frame.rows), rgb_frame.cols);
        cv::Mat gray_frame;
        cv::cvtColor(rgb_frame, gray_frame, CV_RGB2GRAY);
        mimFrameRGB.copy_from(cvd_rgb_frame);
        CVD::SubImage<CVD::byte> cvd_gray_frame(gray_frame.data,
                                                CVD::ImageRef(gray_frame.cols, gray_frame.rows), rgb_frame.cols);
        mimFrameBW.copy_from(cvd_gray_frame);

        tracker_->TrackFrame(mimFrameBW);
        static std::vector<unsigned char> v_rgb_pixels;
        if (v_rgb_pixels.size() != 3 * clone_rgb.cols * clone_rgb.rows)
          v_rgb_pixels.resize(3 * clone_rgb.cols * clone_rgb.rows);
        SafeCopyRGBImage(v_rgb_pixels, clone_rgb);

        static int frame_count = 0;
        frame_count++;
        if (frame_count == 30)
        {
          frame_count = 0;
        }

        static GVars3::gvar3<int> gvnDrawMap("DrawMap", 0, GVars3::HIDDEN | GVars3::SILENT);
        static GVars3::gvar3<int> gvnDrawAR("DrawAR", 0, GVars3::HIDDEN | GVars3::SILENT);

        bool draw_map = map_->IsGood() && *gvnDrawMap;
        bool draw_ar = map_->IsGood() && *gvnDrawAR;

        if (draw_map)
        {
          map_viewer_->DrawMap(tracker_->GetCurrentPose());
        }
        else
        {
          render_->set_video_frame(&v_rgb_pixels);
          render_->Render();
        }

        //      gl_window_->GetMousePoseUpdate();
        std::string caption;
        if (draw_map)
          caption = map_viewer_->GetMessageForUser();
        else
          caption = tracker_->GetMessageForUser();
        gl_window_->DrawCaption(caption);
        gl_window_->DrawMenus();
        //gl_window_->swap_buffers();
        //gl_window_->HandlePendingEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      catch (std::exception &e)
      {
        printf("SlamWindowCallback:: Oops: %s\n", e.what());
      }
    }

    virtual void keyboard(unsigned char c_key, int x, int y)
    {
      c_pressed_key = c_key;
      switch (c_key)
      {
      case 'r':
        tracker_->Reset();
        break;
      case 27: //escape
        exit(0);
        break;
      case ' ':
        tracker_->AskInitialTrack();
        break;
      default:;
      }
      std::cout << "pressed:" << c_key << std::endl;
    }

  public:
    unsigned char c_pressed_key;

    CVD::Image<CVD::Rgb<CVD::byte> > mimFrameRGB;
    CVD::Image<CVD::byte> mimFrameBW;

    std::shared_ptr<Map> map_;
    std::shared_ptr<MapMaker> map_maker_;
    std::shared_ptr<Tracker> tracker_;
    std::shared_ptr<ATANCamera> camera_;
    std::shared_ptr<MapViewer> map_viewer_;
  };
} // namespace ptam
