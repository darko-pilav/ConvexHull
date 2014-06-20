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
			for (unsigned int j = startIndex; j <= endIndex; ++j)
			{
				if (i == j)
					continue;
				
				(*points)[i]->neighbours.push_back((*points)[j]->Index);
			}
		}

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

	unsigned int initialIndexA = aCurrent->Index;
	unsigned int initialIndexB = bCurrent->Index;

	Point *pointRemovedFromHullA = NULL;
	Point *pointRemovedFromHullB = NULL;

//TODO check edge case of two-point lists
//TODO check edge case of no neighbours
	do
	{
		Vec3D currentPlaneNorm = lastEdge->NormCrossP(*currentEdge);

		Point* testNeighbour;
		double candidateCosAngle = -1;
		Point* candidateNeighbour = NULL;
		bool candidateFound = false;
		bool isCandidateFromA = true;

		unsigned int startIndexA = 0;
		for (unsigned int i = 0; i < aCurrent->neighbours.size(); i++)
		{
			if (aCurrent->neighbours[i] == aLast->Index)
			{
				startIndexA = (aCurrent->neighbours.size() + i - 1) % aCurrent->neighbours.size();
				break;
			}
		}

		for (unsigned int i = 0; i < aCurrent->neighbours.size() - 1; i++)
		{
			//Adding the Size of the vector to the currentIndex because we don't want the index to get negative. The modulo takes care of it afterwards.
			//Subtract i in order to walk in a counter clockwise order.
			unsigned int currentIndex = (aCurrent->neighbours.size() + startIndexA - i) % aCurrent->neighbours.size();

			testNeighbour = (*points)[aCurrent->neighbours[currentIndex]];
			Vec3D candidateEdge = Vec3D(*aCurrent, *testNeighbour);
			Vec3D candidatePlaneNorm = candidateEdge.NormCrossP(*currentEdge);

			double testCosAngle = currentPlaneNorm.DotP(candidatePlaneNorm);

			if (testCosAngle > candidateCosAngle)
			{
				if (candidateNeighbour != NULL)
				{
					if (candidateNeighbour->Index != aLast->Index)
						Decouple(*aCurrent, *aLast);

					pointRemovedFromHullA = candidateNeighbour;
				}

				candidateCosAngle = testCosAngle;
				candidateNeighbour = testNeighbour;
			}
		}


		candidateNeighbour == NULL;
		unsigned int startIndexB = 0;
		for (unsigned int i = 0; i < bCurrent->neighbours.size(); i++)
		{
			if (bCurrent->neighbours[i] == bLast->Index)
			{
				startIndexB = (i + 1) % bCurrent->neighbours.size();
				break;
			}
		}

		for (unsigned int i = 0; i < bCurrent->neighbours.size() - 1; i++)
		{
			unsigned int currentIndex = (startIndexB + i) % bCurrent->neighbours.size();

			testNeighbour = (*points)[bCurrent->neighbours[currentIndex]];
			Vec3D *candidateEdge = &Vec3D(*bCurrent, *testNeighbour);
			Vec3D candidatePlaneNorm = currentEdge->NormCrossP(*candidateEdge);

			double testCosAngle = currentPlaneNorm.DotP(candidatePlaneNorm);

			if (testCosAngle > candidateCosAngle)
			{
				if (candidateNeighbour != NULL)
				{
					if (candidateNeighbour->Index != bLast->Index)
						Decouple(*bCurrent, *bLast);

					pointRemovedFromHullB = candidateNeighbour;
				}

				candidateCosAngle = testCosAngle;
				candidateNeighbour = testNeighbour;
				isCandidateFromA = false;
			}
		}

		if (candidateFound)
		{
			if (isCandidateFromA)
			{
				aLast = aCurrent;

				//InsertNeighbourAfter(bCurrent, candidateNeighbour, aCurrent);

				aCurrent = candidateNeighbour;
			}
			else
			{
				bLast = bCurrent;
				bCurrent = candidateNeighbour;
			}
		}

		lastEdge = currentEdge;
		currentEdge = &Vec3D(*aCurrent, *bCurrent);
	} while (initialIndexA != aCurrent->Index || initialIndexB != bCurrent->Index);

	if (pointRemovedFromHullA != NULL)
		RemoveSubGraph(points, pointsA, pointRemovedFromHullA);
	if (pointRemovedFromHullB != NULL)
		RemoveSubGraph(points, pointsB, pointRemovedFromHullB);

	pointsA->insert(pointsA->end(), pointsB->begin(), pointsB->end());
	delete pointsB;

	return pointsA;
}

//Removes the points from the respective neighbour lists
void Hull3D::Decouple(Point &point1, Point &point2)
{
	if (point1.Index == point2.Index)
		return;

	for (int i = 0; i < point1.neighbours.size(); i++)
	{
		if (point1.neighbours[i] == point2.Index)
		{
			point1.neighbours.erase(point1.neighbours.begin() + i);
			break;
		}
	}

	for (int i = 0; i < point2.neighbours.size(); i++)
	{
		if (point2.neighbours[i] == point1.Index)
		{
			point2.neighbours.erase(point2.neighbours.begin() + i);
			break;
		}
	}
}

void Hull3D::RemoveSubGraph(vector<Point*> *allPoints, vector<Point*> *pointsRemoveFrom, Point* pointInGraph)
{
	//allocate a list of single bits. It is used to mark which points have been visited already
	void* visitMap = calloc(pointsRemoveFrom->size(), 1);

	Point* currentPoint = pointInGraph;

	
	RemoveSubGraphRecursively(allPoints, pointsRemoveFrom, pointInGraph, visitMap);
}

void Hull3D::RemoveSubGraphRecursively(vector<Point*> *allPoints, vector<Point*> *pointsRemoveFrom, Point* currentPoint, void* visitMap)
{
	//check if this point has already been visited. If yes, return.
	if (((unsigned char*)visitMap)[currentPoint->Index / 8] & (1 << (currentPoint->Index % 8)) > 0)
		return;

	//mark this point as already visited
	((unsigned char*)visitMap)[currentPoint->Index / 8] = (1 << (currentPoint->Index % 8));

	for (int i = 0; i < pointsRemoveFrom->size(); i++)
	{
		if ((*pointsRemoveFrom)[i]->Index == currentPoint->Index)
		{
			pointsRemoveFrom->erase(pointsRemoveFrom->begin() + i);
			break;
		}
	}

	for (int i = 0; i < currentPoint->neighbours.size(); i++)
		RemoveSubGraphRecursively(allPoints, pointsRemoveFrom, (*allPoints)[currentPoint->neighbours[i]], visitMap);
}
