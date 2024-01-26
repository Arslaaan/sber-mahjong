#pragma once

#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

struct Card {
    Point point1;
    Point point2;
    int row1; // for point1
    int col1;
    int row2; // for point2
    int col2;
    int lvl;

    void addPoint(Point& p);
};