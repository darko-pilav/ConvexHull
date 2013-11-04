#include "HeapSort.h"


HeapSort::HeapSort()
{
}


HeapSort::~HeapSort()
{
}


void HeapSort::Sort(vector<Point*> *points)
{
	int end = points->size() - 1;
	Heapify(points);

	while (end > 0)
	{
		Swap(points, 0, end);
		end--;

		SiftDown(points, 0, end);
	}
}

void HeapSort::Heapify(vector<Point*> *points)
{
	int start = (points->size() - 2) / 2;

	while (start >= 0)
	{
		SiftDown(points, start, points->size() - 1);
		start--;
	}
}


void HeapSort::SiftDown(vector<Point*> *points, int startIndex, int endIndex)
{
	int rootIndex = startIndex;
	int childIndex, swapIndex;
	while (rootIndex * 2 + 1 <= endIndex)
	{
		childIndex = rootIndex * 2 + 1;
		swapIndex = rootIndex;

		if ((*points)[swapIndex]->X < (*points)[childIndex]->X)
			swapIndex = childIndex;
		if (childIndex + 1 <= endIndex && (*points)[swapIndex]->X < (*points)[childIndex + 1]->X)
			swapIndex = childIndex + 1;
		if (swapIndex != rootIndex)
		{
			Swap(points, rootIndex, swapIndex);
			rootIndex = swapIndex;
		}
		else
			return;
	}
}

void HeapSort::Swap(vector<Point*> *points, int firstIndex, int secondIndex)
{
	Point* tmpPoint = (*points)[firstIndex];
	(*points)[firstIndex] = (*points)[secondIndex];
	(*points)[secondIndex] = tmpPoint;
}
