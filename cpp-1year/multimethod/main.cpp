#include <iostream>
#include "shapes.h"
#include "intersects.h"

int main() {


    Shape* t = new Triangle;
    Shape* r = new Rectangle;
    Shape* c = new Circle;
   

    intersect(t, t);
    intersect(t, c);
    intersect(t, r);

    std::cout << std::endl;

    intersect(r, t);
    intersect(r, c);
    intersect(r, r);

    std::cout << std::endl;

    intersect(c, t);
    intersect(c, c);
    intersect(c, r);

    delete t;
    delete r;
    delete c;

    return 0;
}

