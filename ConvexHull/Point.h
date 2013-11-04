#pragma once
class Point
{
public:
	Point();
	Point(double x, double y, double z);
	~Point();

	double X;
	double Y;
	double Z;

private:
	void Init(double x, double y, double z);
};

