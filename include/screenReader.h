#pragma once
#include "card.h"
#include "figureImages.h"
using namespace cv;

class ScreenReader {
    Mat gameScreen;
    Mat gref;
    const int maxErrorOnSameLvl = 5;  // pixels
    const int maxErrorOnSameRowOrCol = 15;  // pixels
    const FigureImages& gameObjectImages_;
    const std::string defaultPath = "../screen.png";

    std::pair<int, int> detectRowAndCol(int x, int y);
    bool inRange(int value, int prop);
    

   public:
    std::vector<std::vector<int>> grid;
    ScreenReader(const FigureImages& gameObjectImages);

    void readFromPhone();
    void read(const std::string& path);
};