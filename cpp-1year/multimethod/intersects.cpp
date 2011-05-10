#include <iostream>
#include <stdexcept>
#include "shapes.h"
#include "intersects.h"

void intersect(Shape* a, Shape* b) {

    if (Triangle * p1 = dynamic_cast<Triangle*> (a)) {
        if (Triangle * p2 = dynamic_cast<Triangle*> (b)) {
            intersect(p1, p2);
        } else if (Circle * p2 = dynamic_cast<Circle*> (b)) {
            intersect(p1, p2);
        } else if (Rectangle * p2 = dynamic_cast<Rectangle*> (b)) {
            intersect(p1, p2);
        } else {
            throw std::logic_error("Undefined intersect!");
        }
    }
    else if (Rectangle * p1 = dynamic_cast<Rectangle*> (a)) {
        if (Triangle * p2 = dynamic_cast<Triangle*> (b)) {
            intersect(p2, p1);
        } else if (Circle * p2 = dynamic_cast<Circle*> (b)) {
            intersect(p1, p2);
        } else if (Rectangle * p2 = dynamic_cast<Rectangle*> (b)) {
            intersect(p1, p2);
        } else {
            throw std::logic_error("Undefined intersect!");
        }
    } else if (Circle * p1 = dynamic_cast<Circle*> (a)) {
        if (Triangle * p2 = dynamic_cast<Triangle*> (b)) {
            intersect(p2, p1);
        } else if (Circle * p2 = dynamic_cast<Circle*> (b)) {
            intersect(p1, p2);
        } else if (Rectangle * p2 = dynamic_cast<Rectangle*> (b)) {
            intersect(p2, p1);
        } else {
            throw std::logic_error("Undefined intersect!");
        }
    } else {
         throw std::logic_error("Undefined intersect!");
    }
}

void intersect(Triangle* a, Triangle * b) {
    std::cout << "Triangle intersects Triangle" << std::endl;
}

void intersect(Triangle* a, Circle * b) {
    std::cout << "Triangle intersects Circle" << std::endl;
}

void intersect(Triangle* a, Rectangle * b) {
    std::cout << "Triangle intersects Rectangle" << std::endl;
}

void intersect(Rectangle* a, Rectangle * b) {
    std::cout << "Rectangle intersects Rectangle" << std::endl;
}

void intersect(Rectangle* a, Circle * b) {
    std::cout << "Rectangle intersects Circle" << std::endl;
}

void intersect(Circle* a, Circle * b) {
    std::cout << "Circle intersects Circle" << std::endl;
}

