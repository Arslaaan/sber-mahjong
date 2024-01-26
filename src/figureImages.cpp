#include "figureImages.h"

FigureImages::FigureImages() {
    const std::filesystem::path dir{"../game objects/"};
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        Mat gameObject = imread(entry.path().string(), IMREAD_COLOR);
        if (gameObject.empty()) {
            std::cout << "Error load game object " + entry.path().filename().string() << std::endl;
        }
        Mat gtpl;
        cvtColor(gameObject, gtpl, COLOR_BGR2GRAY);
        Canny(gtpl, gtpl, 110, 330);
        gameObjects[entry.path().filename().string()] = std::move(gameObject);
    }
}

const std::map<std::string, Mat>& FigureImages::getImages() const {
    return gameObjects;
}
