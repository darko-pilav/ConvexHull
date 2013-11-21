#include "Point.h"


Point::Point()
{
	Init(0, 0, 0, 0);
}

Point::Point(double x, double y, double z, int index)
{
	Init(x, y, z, index);
}

Point::~Point()
{
}

void Point::Init(double x, double y, double z, int index)
{
	X = x;
	Y = y;
	Z = z;

	Index = index;
}
