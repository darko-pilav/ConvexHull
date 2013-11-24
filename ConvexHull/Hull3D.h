#pragma once

#include <vector>

#include "Hull2D.h"
#include "Vec3D.h"
#include "Point.h"

class Hull3D
{
public:
	Hull3D();
	~Hull3D();
	vector<Point*>* FindHull(vector<Point*>* points);

private:

	//Method called recursively to divide set until case is trivial and then recombine subsets.
	vector<Point*>* FindHullOfSubset(vector<Point*> *points, unsigned int startIndex, unsigned int endIndex);

	vector<Point*>* Merge(vector<Point*> *points, vector<Point*> *pointsA, vector<Point*> *pointsB);

	// Returns the Cosinus between two planes. There is no reason to calculate the angle since we are only interested in the relative convex angle => the larger the angle, the smaller the cos
	static double FindCosBetweenPlanes(Vec3D* normalA, Vec3D* edge1B, Vec3D* edge2B)
	{
		Vec3D normalB = edge1B->NormCrossP(*edge2B);
		return normalA->DotP(*normalA);
	}
};

