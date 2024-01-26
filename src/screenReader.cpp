#include "screenReader.h"

ScreenReader::ScreenReader(const FigureImages& gameObjectImages)
    : gameObjectImages_(gameObjectImages) {}

void ScreenReader::readFromPhone() {
    const std::string rmCmd = "rm ";
    const std::string printScreenCmd = "~/platform-tools/adb exec-out screencap -p >";
    system((rmCmd + defaultPath).c_str());
    system((printScreenCmd + defaultPath).c_str());
    read(defaultPath);
}

void ScreenReader::read(const std::string& path) {
    gameScreen = imread(path, IMREAD_COLOR);
    if (gameScreen.empty()) {
        std::cout << "Error load screen.png" << std::endl;
    } else {
        cvtColor(gameScreen, gref, COLOR_BGR2GRAY);
        Canny(gref, gref, 110, 330);
    }

    std::unordered_map<std::string, Card>
        detectedObjects;  // obj -> {point1, point2}
    std::vector<std::vector<int>> gridRaw = {{}, {}};
    std::vector<std::vector<int>> grid = {{}, {}};

    for (const auto& gameObject : gameObjectImages_.getImages()) {
        Mat res_32f(gameScreen.rows - gameObject.second.rows + 1,
                    gameScreen.cols - gameObject.second.cols + 1, CV_32FC1);
        matchTemplate(gameScreen, gameObject.second, res_32f, TM_CCOEFF_NORMED);

        Mat res;
        res_32f.convertTo(res, CV_8U, 255.0);

        int size = ((gameObject.second.cols + gameObject.second.rows) / 4) * 2 +
                   1;  // force size to be odd
        adaptiveThreshold(res, res, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,
                          size, -160);

        double minVal, maxVal = 1;
        Point minLoc, maxLoc;
        while (maxVal > 0) {
            minMaxLoc(res, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
            if (maxVal > 0) {
                floodFill(res, maxLoc, 0);
                Point point(maxLoc.x + gameObject.second.cols,
                            maxLoc.y + gameObject.second.rows);
                std::cout << point << std::endl;
                detectedObjects[gameObject.first].addPoint(point);
                gridRaw[0].push_back(point.x);
                gridRaw[1].push_back(point.y);
            }
        }
    }

    for (int i = 0; i < 2; ++i) {
        std::sort(gridRaw[i].begin(), gridRaw[i].end());
        int prev = gridRaw[i][0];
        grid[i].push_back(prev);
        for (int j = 1; j < gridRaw[i].size(); ++j) {
            if (gridRaw[i][j] - prev > this->maxErrorOnSameLvl) {
                grid[i].push_back(gridRaw[i][j]);
            }
            prev = gridRaw[i][j];
        }
    }
    for (int i = 0; i < 2; ++i) {
        for (const auto& it : grid[i]) {
            std::cout << it << ", ";
        }
        std::cout << std::endl;
    }
    // определить высоты карточек
    // определить - это новая строка или нет
    // если новая - наибольший уровень
    // если старая - уровень - 1. прочекать, что действительно самые высокие карты - у них меньше координата
    for (const auto& detectedObjectEntry : detectedObjects) {
        // todo implement later binary search for optimization
        for (int i = 0; i < 2; ++i) {
            int currentRowOrCol = 0;
            // обработать первый элемент
            for (int j = 1; j < grid[i].size(); ++j) {
                if (grid[i][j] - grid[i][j - 1] >
                    this->maxErrorOnSameRowOrCol) {
                    // новая строка или столбец
                    currentRowOrCol++;
                } else {
                    // та же строка или столбец
                    // тут возможно понадобится корректировка, если макс высота уровней 
                }
                // prev = gridRaw[i][j];
            }
        }
    }
}

// void ScreenReader::calibrate(const FigureImages& gameObjectImages) {
//     std::vector<std::vector<int>> gridRaw = {{}, {}};
//     for (const auto& gameObject : gameObjectImages.getImages()) {
//         Mat gameScreenCopy = gameScreen.clone();
//         Mat res_32f(gameScreenCopy.rows - gameObject.second.rows + 1,
//                     gameScreenCopy.cols - gameObject.second.cols + 1,
//                     CV_32FC1);
//         matchTemplate(gameScreenCopy, gameObject.second, res_32f,
//                       TM_CCOEFF_NORMED);

//         Mat res;
//         res_32f.convertTo(res, CV_8U, 255.0);

//         int size = ((gameObject.second.cols + gameObject.second.rows) / 4) *
//         2 +
//                    1;  // force size to be odd
//         adaptiveThreshold(res, res, 255, ADAPTIVE_THRESH_MEAN_C,
//         THRESH_BINARY,
//                           size, -160);

//         double minVal, maxVal = 1;
//         Point minLoc, maxLoc;
//         while (maxVal > 0) {
//             minMaxLoc(res, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
//             if (maxVal > 0) {
//                 floodFill(res, maxLoc, 0);
//                 Point point(maxLoc.x + gameObject.second.cols,
//                             maxLoc.y + gameObject.second.rows);
//                 gridRaw[0].push_back(point.x);
//                 gridRaw[1].push_back(point.y);
//                 // rectangle(gameScreenCopy, maxLoc, point, Scalar(0, 255,
//                 0),
//                 // 2);
//             }
//         }

//         // namedWindow(gameObject.first, WINDOW_NORMAL);
//         // imshow(gameObject.first, gameScreenCopy);
//         // resizeWindow(gameObject.first, 800, 800);
//         // waitKey(0);
//     }

//     for (int i = 0; i < 2; ++i) {
//         grid[i].clear();
//         std::sort(gridRaw[i].begin(), gridRaw[i].end());
//         int prev = gridRaw[i][0];
//         grid[i].push_back(prev);
//         for (int j = 1; j < gridRaw[i].size(); ++j) {
//             if (gridRaw[i][j] - prev > this->error) {
//                 grid[i].push_back(gridRaw[i][j]);
//             }
//             prev = gridRaw[i][j];
//         }
//     }
//     for (int i = 0; i < 2; ++i) {
//         for (const auto& it : grid[i]) {
//             std::cout << it << ", ";
//         }
//         std::cout << std::endl;
//     }
// }

bool ScreenReader::inRange(int value, int prop) {
    return value >= prop - maxErrorOnSameLvl &&
           value <= prop + maxErrorOnSameLvl;
}

std::pair<int, int> ScreenReader::detectRowAndCol(int x, int y) {
    // todo can be optimized
    // int row = -1, col = -1;
    // for (int i = 0; i < COL_SIZE; ++i) {
    //     if (inRange(x, grid[0][i])) {
    //         col = i;
    //     }
    // }
    // for (int i = 0; i < ROW_SIZE; ++i) {
    //     if (inRange(y, grid[1][i])) {
    //         row = i;
    //     }
    // }
    // return {row, col};
    return {0, 0};
}