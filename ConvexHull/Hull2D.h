#pragma once

#include <vector>
#include "Point.h"

using namespace std;

class Hull2D
{
public:
	Hull2D();
	~Hull2D();
	static vector<Point*>* FindHull(vector<Point*> *points);
	static void FindTangentIndex(vector<Point*> *pointsA, unsigned int &aTangentIndex, vector<Point*> *pointsB, unsigned int &bTangentIndex, bool isLeftTangent);

private:
	static vector<Point*>* FindHullOfSubset(vector<Point*> *points, unsigned int startIndex, unsigned int stopIndex);

	static vector<Point*>* Merge(vector<Point*> *pointsA, vector<Point*> *pointsB);

	static void FindOutermostPoints(vector<Point*> *points, unsigned int& leftmostIndex, unsigned int& rightmostIndex);
	static void FindTangent(vector<Point*> *pointsA, unsigned int& aLeftmostIndex, unsigned int aOtherIndex, vector<Point*> *pointsB, unsigned int& bRightmostIndex, unsigned int bOtherIndex, bool isLeftTangent);

	static bool SeekTangentIndex(vector<Point*> *points, unsigned int *index, unsigned int lastIndex, double slopeAB);

	static inline double CalculateSlope(vector<Point*> *points, unsigned int startIndex)
	{
		return CalculateSlope(points, startIndex, points, (startIndex + 1) % points->size());
	};
	static inline double CalculateSlope(vector<Point*> *pointsA, unsigned int indexA, vector<Point*> *pointsB, unsigned int indexB)
	{
		return ((*pointsA)[indexA]->Y - (*pointsB)[indexB]->Y) / ((*pointsA)[indexA]->X - (*pointsB)[indexB]->X);
	};

};

