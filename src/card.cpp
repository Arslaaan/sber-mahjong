#include "card.h"

void Card::addPoint(Point& p) {
    if (point1.x == 0 && point1.y == 0) {
        point1 = p;
    } else {
        point2 = p;
    }
}