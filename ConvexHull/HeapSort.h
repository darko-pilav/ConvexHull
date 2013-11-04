#pragma once
#include <vector>

#include "Point.h"

using namespace std;

class HeapSort
{
public:
	HeapSort();
	~HeapSort();
	void Sort(vector<Point*> *points);

private:
	void Heapify(vector<Point*> *points);
	void SiftDown(vector<Point*> *points, int startIndex, int endIndex);
	void Swap(vector<Point*> *points, int firstIndex, int secondIndex);
};

