#include <iostream>
#include <map>
#include <vector>

#include "figureImages.h"
#include "screenReader.h"

using namespace cv;

int main() {
    FigureImages gameObjectImages;
    ScreenReader screenReader(gameObjectImages);

    screenReader.readFromPhone();

    return 0;
}