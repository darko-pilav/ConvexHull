#include <vector>
#include "Hull2D.h"

using namespace std;

Hull2D::Hull2D()
{
}


Hull2D::~Hull2D()
{
}

vector<Point*>* Hull2D::FindHull(vector<Point*> *points)
{
	return FindHullOfSubset(points, 0, points->size() - 1);
}

//Method called recursively to dividing set until case is trivial and the recombine subsets.
vector<Point*>* Hull2D::FindHullOfSubset(vector<Point*> *points, int startIndex, int endIndex)
{

	if (endIndex - startIndex > 2) 	//if more than three Points in set, subdivide it and merge the subdivision.
	{
		int midIndex = startIndex + (endIndex - startIndex) / 2;
		return Merge(FindHullOfSubset(points, startIndex, midIndex), FindHullOfSubset(points, midIndex + 1, endIndex));
	}
	else //if three or less points, we have a trivial case -> add to temporary hull
	{
		vector<Point*>* hullPoints = new vector<Point*>();

		bool invertOrder = false;
		//If there are exactly three points, check the orinetation of triangle. Make sure we have always a cockwise orientation.
		if (endIndex - startIndex == 2)
		{
			double slope1 = CalculateSlope(points, startIndex);
			double slope2 = CalculateSlope(points, startIndex + 1);
			if (slope1 < slope2)
				invertOrder = true;
		}

		//Add trivial case to temporary Hull
		if (invertOrder == false)
		{
			for (int i = startIndex; i <= endIndex; ++i)
				hullPoints->push_back((*points)[i]);
		}
		else
		{
			for (int i = endIndex; i >= startIndex; --i)
				hullPoints->push_back((*points)[i]);
		}

		return hullPoints;
	}
}


vector<Point*>* Hull2D::Merge(vector<Point*> *pointsA, vector<Point*> *pointsB)
{
	int aLeftTangentIndex, aRightTangentIndex;
	int bLeftTangentIndex, bRightTangentIndex;

	//Leftmost and rightmost points in set are the starting/ending points for tangent-point searches.
	FindOutermostPoints(pointsA, aLeftTangentIndex, aRightTangentIndex);
	FindOutermostPoints(pointsB, bLeftTangentIndex, bRightTangentIndex);

	//The left and right indices are passed as references and can therefore change. 
	//Since we need the original value as an endpoint of the search, we copy the values
	int aOrigLeft = aLeftTangentIndex;
	int aOrigRight = aRightTangentIndex;
	int bOrigLeft = bLeftTangentIndex;
	int bOrigRight = bRightTangentIndex;

	//Search for left/right tangent writes directly into the parameters passed as a reference.
	FindTangent(pointsA, aLeftTangentIndex, aOrigRight, pointsB, bLeftTangentIndex, bOrigRight, true);
	FindTangent(pointsA, aRightTangentIndex, aOrigLeft, pointsB, bRightTangentIndex, bOrigLeft, false);

	//merged hull points vector
	vector<Point*> *hullPoints = new vector<Point*>();

	hullPoints->push_back((*pointsA)[aRightTangentIndex]);

	bool inSetA = true;
	int currentIndex = aRightTangentIndex;
	//Walk through all points in hull and change from hull A to hull B when appropriate.
	while ((inSetA == false && currentIndex == bRightTangentIndex) == false)
	{
		if (inSetA && (currentIndex == aLeftTangentIndex))
		{
			inSetA = false;
			currentIndex = bLeftTangentIndex;
		}
		else
			currentIndex = (currentIndex + 1) % (inSetA ? pointsA->size() : pointsB->size());

		hullPoints->push_back((inSetA ? *pointsA : *pointsB)[currentIndex]);
	}

	//cleanup
	delete pointsA;
	delete pointsB;

	return hullPoints;
}

void Hull2D::FindOutermostPoints(vector<Point*> *points, int& leftmostPointIndex, int& rightmostPointIndex)
{
	//simple walk through all values and find left and right outermost points
	leftmostPointIndex = 0; rightmostPointIndex = points->size() - 1;
	for (unsigned int i = 0; i < (*points).size(); ++i)
	{
		if ((*points)[i]->X < (*points)[leftmostPointIndex]->X)
			leftmostPointIndex = i;
		if ((*points)[i]->X > (*points)[rightmostPointIndex]->X)
			rightmostPointIndex = i;
	}
}

void Hull2D::FindTangent(vector<Point*> *pointsA, int& aRightMostIndex, int aOtherIndex, vector<Point*> *pointsB, int& bRightMostIndex, int bOtherIndex, bool isLeftTangent)
{
	vector<Point*>* workingSetA;
	vector<Point*>* workingSetB;
	int* workingIndexA;
	int* workingIndexB;
	int lastIndexA, lastIndexB;

	//Depending on which tangent we are looking for, a different order of the iteration is necessary
	//Simply swap A and B if neccessary
	workingSetA = isLeftTangent ? pointsB : pointsA;
	workingSetB = isLeftTangent ? pointsA : pointsB;
	workingIndexA = isLeftTangent ? &bRightMostIndex : &aRightMostIndex;
	lastIndexA = isLeftTangent ? bOtherIndex : aOtherIndex;
	workingIndexB = isLeftTangent ? &aRightMostIndex : &bRightMostIndex;
	lastIndexB = isLeftTangent ? aOtherIndex : bOtherIndex;


	float slopeA;
	bool aChanged = true;
	float slopeB;
	bool bChanged = true;
	float slopeAB;

	bool foundBIndex = false;
	while (foundBIndex == false)
	{
		bool foundAIndex = false;
		while (foundAIndex == false)
		{
			slopeAB = CalculateSlope(workingSetA, *workingIndexA, workingSetB, *workingIndexB);
			
			foundAIndex = SeekTangentIndex(workingSetA, workingIndexA, lastIndexA, slopeAB);
		}

		foundBIndex = SeekTangentIndex(workingSetB, workingIndexB, lastIndexB, slopeAB);
	}
}

bool Hull2D::SeekTangentIndex(vector<Point*> *points, int *index, int lastIndex, double slopeAB)
{
	double slope = CalculateSlope(points, *index);

	if (*index != lastIndex && slope >= slopeAB) 		//make sure to wrap around
	{
		*index = (*index + 1) % points->size();
		return false;
	}
	else
		return true;
}
