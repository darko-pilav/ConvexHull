#include <windows.h>  // For MS Windows
#include <GL\glut.h>  // GLUT, includes glu.h and gl.h
#include <stdio.h>
#include <iostream>
#define _USE_MATH_DEFINES // for C++
#include <math.h>
#include <random>
#include <vector>

#include "Point.h"
#include "HeapSort.h"
#include "Hull2D.h"
#include "Hull3D.h"

using namespace std;


vector<Point*>* points = new vector<Point*>();
vector<Point*>* hull2D;
vector<Point*>* hull3D;

//Handler for window-repaint event. Call back when the window first appears and
//whenever the window needs to be re-painted.
void Display() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	// Draw a Red 1x1 Square centered at origin
	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f);	// Red
	for (int i = 0; i < points->size(); i++)
	{
		glVertex3f((*points)[i]->X - 0.001f, (*points)[i]->Y - 0.001f, (*points)[i]->Z - 0.001f);    // x, y, z
		glVertex3f((*points)[i]->X + 0.001f, (*points)[i]->Y - 0.001f, (*points)[i]->Y - 0.001f);    // x, y, z
		glVertex3f((*points)[i]->X + 0.001f, (*points)[i]->Y + 0.001f, (*points)[i]->Y - 0.001f);    // x, y, z
		glVertex3f((*points)[i]->X - 0.001f, (*points)[i]->Y + 0.001f, (*points)[i]->Y - 0.001f);    // x, y, z
	}
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);              // Each set of 4 vertices form a quad
	glColor3f(0.0f, 1.0f, 0.0f); // Red
	for (int i = 0; i < hull2D->size(); i++)
	{
		glVertex2f((*hull2D)[i]->X, (*hull2D)[i]->Y);    // x, y
	}
	glEnd();


	glFlush();  // Render now
}



int main(int argc, char** argv)
{
	srand(5);
	HeapSort sorter = HeapSort();
	Hull2D hullFinder2D = Hull2D();
	Hull3D hullFinder3D = Hull3D();

	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	LARGE_INTEGER t3, t4;           // ticks
	double elapsedTime;
	
	QueryPerformanceFrequency(&frequency);

	for (unsigned int i = 0; i < 10 * pow(2, 0); i++)
	{
		double theta = (float)rand() / RAND_MAX * M_PI * 2;
		double phi = (float)rand() / RAND_MAX * M_PI * 2;
		double r = (float)rand() / RAND_MAX * 0.5f;
		points->push_back(new Point(sin(theta) * cos(phi) * r, sin(theta) * sin(phi) * r, cos(theta) * r, points->size()));
	}

	cout << endl;

	cout << "Started Sorting..." << endl;

	QueryPerformanceCounter(&t1);
	sorter.Sort(points);
	QueryPerformanceCounter(&t2);
		
	cout << "Finished Sorting in " << (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart << "ms ..." << endl << endl;

	cout << endl << "Started Searching for Hull..." << endl;
		
	QueryPerformanceCounter(&t3);
	hull2D = hullFinder2D.FindHull(points);
	QueryPerformanceCounter(&t4);

	cout << "Finished Searching for Hull in " << (t4.QuadPart - t3.QuadPart) * 1000.0 / frequency.QuadPart << "ms ..." << endl;
	cout << endl;
	cout << "Number of points: " << points->size() << ", Number of points in hull: " << hull2D->size() << ", Total time: " << (t2.QuadPart - t1.QuadPart + t4.QuadPart - t3.QuadPart) * 1000.0 / frequency.QuadPart << "ms" << endl;



	glutInit(&argc, argv);					// Initialize GLUT
	glutCreateWindow("OpenGL Setup Test");	// Create a window with the given title
	glutInitWindowSize(1024, 768);			// Set the window's initial width & height
	glutInitWindowPosition(250, 250);		// Position the window's initial top-left corner
	glutDisplayFunc(Display);				// Register display callback handler for window re-paint
	glutMainLoop();							// Enter the infinitely event-processing loop
	return 0;
}