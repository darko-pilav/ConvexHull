#include <vector>
#include "Point.h"

#pragma once
using namespace std;

class Hull2D
{
public:
	Hull2D();
	~Hull2D();
	vector<Point*>* FindHull(vector<Point*> *points);

private:
	vector<Point*>* FindHullOfSubset(vector<Point*> *points, int startIndex, int stopIndex);

	vector<Point*>* Merge(vector<Point*> *pointsA, vector<Point*> *pointsB);

	void FindOutermostPoints(vector<Point*> *points, int& leftmostIndex, int& rightmostIndex);
	void FindTangent(vector<Point*> *pointsA, int& aLeftmostIndex, int aOtherIndex, vector<Point*> *pointsB, int& bRightmostIndex, int bOtherIndex, bool isLeftTangent);

	bool SeekTangentIndex(vector<Point*> *points, int *index, int lastIndex, double slopeAB);

	inline double CalculateSlope(vector<Point*> *points, int startIndex)
	{
		return CalculateSlope(points, startIndex, points, (startIndex + 1) % points->size());
	};
	inline double CalculateSlope(vector<Point*> *pointsA, int indexA, vector<Point*> *pointsB, int indexB)
	{
		return ((*pointsA)[indexA]->Y - (*pointsB)[indexB]->Y) / ((*pointsA)[indexA]->X - (*pointsB)[indexB]->X);
	};

};

