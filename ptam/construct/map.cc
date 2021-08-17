// Copyright 2008 Isis Innovation Limited
#include "ptam/construct/map.h"
#include "ptam/construct/map_point.h"

namespace ptam {
Map::Map() {
  Reset();
}

void Map::Reset() {
  if(points.size() > 0) printf("!!! Map::Reset(): Deleting %d points\n", points.size());
  for (unsigned int i = 0; i < points.size(); i++)
    delete points[i];
  points.clear();
  good = false;
  EmptyTrash();
}

void Map::MoveBadPointsToTrash() {
  int nBad = 0;
  for (int i = points.size()-1; i >= 0; i--) {
    if (points[i]->bBad) {
      trashed_points.push_back(points[i]);
      points.erase(points.begin() + i);
      nBad++;
    }
  }
}

void Map::EmptyTrash() {
  if(trashed_points.size() > 0) printf("!!! Map::EmptyTrash(): Deleting %d trashed points\n", trashed_points.size());
  for (unsigned int i = 0; i < trashed_points.size(); i++)
    delete trashed_points[i];
  trashed_points.clear();
}
}  // namespace ptam
