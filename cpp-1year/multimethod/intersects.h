
#ifndef _INTERSECTS_H
#define	_INTERSECTS_H

#include "shapes.h"

void intersect(Shape* a, Shape* b);

void intersect(Triangle* a, Triangle* b);
void intersect(Triangle* a, Circle* b);
void intersect(Triangle* a, Rectangle* b);
void intersect(Rectangle* a, Rectangle* b);
void intersect(Rectangle* a, Circle* b);
void intersect(Circle* a, Circle* b);

#endif	/* _INTERSECTS_H */

