#include "Hull3D.h"


Hull3D::Hull3D()
{
}


Hull3D::~Hull3D()
{
}


vector<Point*>* Hull3D::FindHull(vector<Point*>* points)
{
	return FindHullOfSubset(points, 0, points->size() - 1);
}


vector<Point*>* Hull3D::FindHullOfSubset(vector<Point*> *points, unsigned int startIndex, unsigned int endIndex)
{
	if (endIndex - startIndex > 2) 	//if more than four Points in set, subdivide it and merge the subdivision.
	{
		unsigned int midIndex = startIndex + (endIndex - startIndex) / 2;
		return Merge(points, FindHullOfSubset(points, startIndex, midIndex), FindHullOfSubset(points, midIndex + 1, endIndex));
	}
	else //if four or less points, we have a trivial case -> add to temporary hull
	{
		vector<Point*>* hullPoints = new vector<Point*>();

		//Add trivial case to temporary Hull
		for (unsigned int i = startIndex; i <= endIndex; ++i)
		{
			hullPoints->push_back((*points)[i]);
			for (unsigned int j = startIndex; j <= endIndex; ++i)
			{
				if (i == j)
					continue;
				
				(*points)[i]->neighbours.push_back((*points)[j]->Index);
			}
		}
		/*
		//If there are exactly four points, We have to check the orientation of the neighbour lists.
		if (endIndex - startIndex == 3)
		{
			for (unsigned int i = 0; i <= hullPoints->size(); ++i)
			{
				Point* currentPoint = (*hullPoints)[i];
				int neighbourIndex1 = currentPoint->neighbours[0];
				int neighbourIndex2 = currentPoint->neighbours[1];
				int neighbourIndex3 = currentPoint->neighbours[2];
				Vec3D planeNormVec = Vec3D::CrossP(Vec3D(*(*hullPoints)[i], *(*points)[neighbourIndex1]), Vec3D(*(*hullPoints)[i], *(*points)[neighbourIndex2]));

				double sideIndicator = Vec3D(*(*hullPoints)[i], *(*points)[neighbourIndex3]).DotP(planeNormVec);
				if (sideIndicator >)

			}

		}*/


		return hullPoints;
	}
}

vector<Point*>* Hull3D::Merge(vector<Point*> *points, vector<Point*> *pointsA, vector<Point*> *pointsB)
{
	unsigned int aRightTangent, bRightTangent;
	Hull2D::FindTangentIndex(pointsA, aRightTangent, pointsB, bRightTangent, false);
	Point *aLast = (*pointsA)[aRightTangent], *aCurrent = (*pointsA)[aRightTangent];
	Point *bLast = (*pointsB)[bRightTangent], *bCurrent = (*pointsB)[bRightTangent];

	Vec3D *lastEdge = &Vec3D(0, 0, -1);
	Vec3D *currentEdge = &Vec3D(*(*pointsA)[aRightTangent], *(*pointsB)[bRightTangent]);

	Vec3D currentPlaneNorm = lastEdge->NormCrossP(*currentEdge);

	Point* testNeighbour;
	unsigned int neighbourStartIndex;
	double candidateCosAngle = 1;
	Point* candidateNeighbour;
	bool isCandidateFromA = true;
	for (unsigned int i = 0; i < aCurrent->neighbours.size(); i++)
	{
		testNeighbour = (*points)[aCurrent->neighbours[i]];
		Vec3D *candidateEdge = &Vec3D(*aCurrent, *testNeighbour);
		Vec3D candidatePlaneNorm = currentEdge->NormCrossP(*candidateEdge);

		double testCosAngle = currentPlaneNorm.DotP(candidatePlaneNorm);

		if (testCosAngle > candidateCosAngle)
		{
			candidateCosAngle = testCosAngle;
			candidateNeighbour = testNeighbour;
		}
	}

	for (unsigned int i = 0; i < bCurrent->neighbours.size(); i++)
	{
		testNeighbour = (*points)[bCurrent->neighbours[i]];
		Vec3D *candidateEdge = &Vec3D(*bCurrent, *testNeighbour);
		Vec3D candidatePlaneNorm = currentEdge->NormCrossP(*candidateEdge);

		double testCosAngle = currentPlaneNorm.DotP(candidatePlaneNorm);

		if (testCosAngle > candidateCosAngle)
		{
			candidateCosAngle = testCosAngle;
			candidateNeighbour = testNeighbour;
			isCandidateFromA = false;
		}
	}

	if (isCandidateFromA)
	{
		aLast = aCurrent;
		aCurrent = candidateNeighbour;
	}
	else
	{
		bLast = bCurrent;
		bCurrent = candidateNeighbour;
	}
	lastEdge = currentEdge;
	currentEdge = &Vec3D(*aCurrent, *bCurrent);
}

/*
double Hull3D::FindNeighbourWithLargestAngle(Vec3D *normalOfTestPlane, Vec3D *currentEdge, Point *testNeighbours, vector<Point*> *allPoints, bool testClockwise)
{
	unsigned int startingPointIndex;

	for (unsigned int i = )
}*/