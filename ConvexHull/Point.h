#pragma once

#include <vector>
#include "Vec3D.h"

using namespace std;

class Point : public Vec3D
{
public:
	Point();
	Point(double x, double y, double z, int index);
	~Point();

	int Index;

	vector<int> neighbours;

	operator Vec3D()
	{
		return Vec3D(X, Y, Z);
	}

private:
	void Init(double x, double y, double z, int index);
};

