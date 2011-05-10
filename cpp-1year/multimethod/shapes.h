#ifndef _SHAPES_H
#define	_SHAPES_H

class Shape {

public:
    virtual ~Shape() {}
};

class Triangle : public Shape {
};

class Rectangle : public Shape {
};

class Circle : public Shape {
};

#endif	/* _SHAPES_H */

