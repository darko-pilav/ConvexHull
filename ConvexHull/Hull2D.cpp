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
	unsigned int maxSizeIndex = points->size() - 1;
	return FindHullOfSubset(points, 0, maxSizeIndex);
}



void Hull2D::FindTangentIndex(vector<Point*> *pointsA, unsigned int &aTangentIndex, vector<Point*> *pointsB, unsigned int &bTangentIndex, bool isLeftTangent)
{
	//Leftmost and rightmost points in set are the starting/ending points for tangent-point searches.
	unsigned int leftIndex, rightIndex;
	FindOutermostPoints(pointsA, leftIndex, rightIndex);
	aTangentIndex = (isLeftTangent ? leftIndex : rightIndex);
	FindOutermostPoints(pointsB, leftIndex, rightIndex);
	bTangentIndex = (isLeftTangent ? leftIndex : rightIndex);

	//The left and right indices are passed as references and can therefore change. 
	//Since we need the original value as an endpoint of the search, we copy the values
	unsigned int aOrig = aTangentIndex;
	unsigned int bOrig = bTangentIndex;

	//Search for left/right tangent writes directly into the parameters passed as a reference.
	FindTangent(pointsA, aTangentIndex, aOrig, pointsB, bTangentIndex, bOrig, isLeftTangent);
}


//Method called recursively to divide set until case is trivial and then recombine subsets.
vector<Point*>* Hull2D::FindHullOfSubset(vector<Point*> *points, int startIndex, unsigned int endIndex)
{
	if (endIndex - startIndex > 2) 	//if more than three Points in set, subdivide it and merge the subdivision.
	{
		unsigned int midIndex = startIndex + (endIndex - startIndex) / 2;
		return Merge(FindHullOfSubset(points, startIndex, midIndex), FindHullOfSubset(points, midIndex + 1, endIndex));
	}
	else //if three or less points, we have a trivial case -> add to temporary hull
	{
		vector<Point*>* hullPoints = new vector<Point*>();

		bool invertOrder = false;
		//If there are exactly three points, check the orinetation of triangle. Make sure we have always a clockwise orientation.
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
			for (unsigned int i = startIndex; i <= endIndex; ++i)
				hullPoints->push_back((*points)[i]);
		}
		else
		{
			for (unsigned int i = startIndex; i <= endIndex; ++i)
				hullPoints->push_back((*points)[endIndex - i]);
		}

		return hullPoints;
	}
}

vector<Point*>* Hull2D::Merge(vector<Point*> *pointsA, vector<Point*> *pointsB)
{
	unsigned int aLeftTangentIndex, aRightTangentIndex;
	unsigned int bLeftTangentIndex, bRightTangentIndex;

	//writes tangent indices directly into the variables which are passed as a reference
	FindTangentIndex(pointsA, aLeftTangentIndex, pointsB, bLeftTangentIndex, true);
	FindTangentIndex(pointsA, aRightTangentIndex, pointsB, bRightTangentIndex, false);

	//merged hull points vector
	vector<Point*> *hullPoints = new vector<Point*>();

	hullPoints->push_back((*pointsA)[aRightTangentIndex]);

	bool inSetA = true;
	unsigned int currentIndex = aRightTangentIndex;
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

void Hull2D::FindOutermostPoints(vector<Point*> *points, unsigned int& leftmostPointIndex, unsigned int& rightmostPointIndex)
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

void Hull2D::FindTangent(vector<Point*> *pointsA, unsigned int& aRightMostIndex, unsigned int aOtherIndex, vector<Point*> *pointsB, unsigned int& bRightMostIndex, unsigned int bOtherIndex, bool isLeftTangent)
{
	vector<Point*>* workingSetA;
	vector<Point*>* workingSetB;
	unsigned int* workingIndexA;
	unsigned int* workingIndexB;
	unsigned int lastIndexA, lastIndexB;

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

bool Hull2D::SeekTangentIndex(vector<Point*> *points, unsigned int *index, unsigned int lastIndex, double slopeAB)
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
