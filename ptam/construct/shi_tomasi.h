// Copyright 2008 Isis Innovation Limited
#pragma once
#include <cvd/image.h>
#include <cvd/byte.h>

namespace ptam {
double FindShiTomasiScoreAtPoint(CVD::BasicImage<CVD::byte> &image,
         int nHalfBoxSize,
         CVD::ImageRef irCenter);
}  // namespace ptam