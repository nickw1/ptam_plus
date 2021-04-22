```
cmake .
make
```
### Build with Emscripten
Install emscripten: https://emscripten.org/docs/getting_started/downloads.html
Build following the CMAKE instructions: https://emscripten.org/docs/compiling/Building-Projects.html?highlight=cmake
```
emcmake cmake .
emmake make
```
Fix math.h include issue with setting the math.h path in cmath to `#include </Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/math.h>`

PTAM Plus
============

PTAM (Parallel Tracking and Mapping) re-released under GPLv3.

#About PTAM

PTAM is a monocular SLAM (Simultaneous Localization and Mapping) system useful for real-time
6-DOF camera tracking in small scenes. It was originally developed as a research system in the Active 
Vision Laboratory of the University of Oxford, as described in the following papers:

- Georg Klein and David Murray, "Parallel Tracking and Mapping for Small AR Workspaces", Proc. ISMAR 2007
- Georg Klein and David Murray, "Improving the Agility of Keyframe-based SLAM", Proc. ECCV 2008

#Previous PTAM release

PTAM was initially released in 2008 by Isis Innovation under a license suitable for
academic use. That version is also available for commercial use subject to a license
agreement with Isis Innovation. That version remains available here:
http://www.robots.ox.ac.uk/~gk/PTAM/

This code represents a re-licensed fork of Isis Innovation's PTAM Source Code Release v1.0-r114.

#About PTAM-Plus

This implementation is from the spirit of
[PTAMM-Plus: Refactoring and Extending PTAMM](http://www.icg.tugraz.at/Members/thanh/publications/ptamm-plus-refactoring-and-extending-ptamm-1).
It is refactored code of [PTAM] (http://www.robots.ox.ac.uk/~gk/PTAM/).
The architect is redesigned a bit in order to overcome monolithic issue of original PTAM.
For instance,  UI code is completely separated from core slam code.

# Dependencies
## "Built-in" dependencies (doesn't require to build externally)
1. [TooN](http://www.edwardrosten.com/cvd/toon.html): included in ~/root/external/TooN
2. [CVD library](http://www.edwardrosten.com/cvd): included in ~/root/external/libcvd
3. [GVars3](http://www.edwardrosten.com/cvd/gvars3.html): included in ~/root/external/gvars-3.0

## External dependencies
4. [OpenCV](http://www.opencv.org) : for `apps/slam` and `apps/calibration` to catpure video
5. [GLUT](http://freeglut.sourceforge.net) : for `apps` with OpenGL need
6. [GLEW](http://glew.sourceforge.net): for windows only
7. [CMake](http://www.cmake.org): for compilation

# Todo
- more cleanup, c++11 code (modern c++).
- Map IO.
- better recovery (features based).

# Need help!
contact: william dot mccohen at gmail dot com