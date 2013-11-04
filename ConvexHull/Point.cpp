#include "Point.h"


Point::Point()
{
	Init(0, 0, 0);
}

Point::Point(double x, double y, double z)
{
	Init(x, y, z);
}

Point::~Point()
{
}

void Point::Init(double x, double y, double z)
{
	X = x;
	Y = y;
	Z = z;
}
