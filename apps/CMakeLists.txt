project(apps)


if(PTAM_WITH_UI)
##-----------------------------------------------------------------------------
# Optional Apps

option(PTAM_APP_CALIBRATION "Build Calibration" ON)
if(PTAM_APP_CALIBRATION)
  # add_subdirectory(calibration)
endif(PTAM_APP_CALIBRATION)

option(PTAM_APP_SLAM "Build Slam" ON)
if(PTAM_APP_SLAM)
  add_subdirectory(slam)
endif(PTAM_APP_SLAM)

endif(PTAM_WITH_UI)
