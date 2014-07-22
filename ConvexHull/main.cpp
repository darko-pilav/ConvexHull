#if defined(_WIN32) || defined(WIN32)
	#include <windows.h>
#elif defined(__linux) || defined(linux)
	#include <sys/time.h>
#endif // WIN32

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <stdio.h>
#include <iostream>
#define _USE_MATH_DEFINES // has to be in front of include <math.h>
#include <math.h>
#include <random>
#include <vector>

#include "Point.h"
#include "HeapSort.h"
#include "Hull2D.h"

using namespace std;

vector<Point*>* points = new vector<Point*>();
vector<Point*>* hull2D;

//Handler for window-repaint event. Call back when the window first appears and
//whenever the window needs to be re-painted.
void Display() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	// Draw a Red 1x1 Square centered at origin
	glBegin(GL_QUADS);              // Drawing quads beacause it is easier to see them/debug.
	glColor3f(1.0f, 0.0f, 0.0f);    // Red
	for (unsigned int i = 0; i < points->size(); i++)
	{
		glVertex2f((*points)[i]->X - 0.001f, (*points)[i]->Y - 0.001f);
		glVertex2f((*points)[i]->X + 0.001f, (*points)[i]->Y - 0.001f);
		glVertex2f((*points)[i]->X + 0.001f, (*points)[i]->Y + 0.001f);
		glVertex2f((*points)[i]->X - 0.001f, (*points)[i]->Y + 0.001f);
	}
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);              // Each set of 4 vertices form a quad
	glColor3f(0.0f, 1.0f, 0.0f); // Red
	for (unsigned int i = 0; i < hull2D->size(); i++)
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

#if defined(_WIN32) || defined(WIN32)
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	LARGE_INTEGER t3, t4;           // ticks
#elif defined(__linux) || defined(linux)
	struct timeval t1, t2, t3, t4;
	struct timezone tz;
#endif
	
#if defined(_WIN32) || defined(WIN32)
	QueryPerformanceFrequency(&frequency);
#endif

	for (unsigned int i = 0; i < 10 * pow(2, 5); i++)
	{
		double phi = (float)rand() / RAND_MAX * M_PI * 2;
		double r = (float)rand() / RAND_MAX * 0.5f;
		points->push_back(new Point(cos(phi) * r, sin(phi) * r, 0));
	}

	cout << endl;

	cout << "Started Sorting..." << endl;

#if defined(_WIN32) || defined(WIN32)
	QueryPerformanceCounter(&t1);
#elif defined(__linux) || defined(linux)
	gettimeofday(&t1, &tz);
#endif
	sorter.Sort(points);
#if defined(_WIN32) || defined(WIN32)
	QueryPerformanceCounter(&t2);
#elif defined(__linux) || defined(linux)
	gettimeofday(&t2, &tz);
#endif

#if defined(_WIN32) || defined(WIN32)
	cout << "Finished Sorting in " << (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart << "ms ..." << endl << endl;
#elif defined(__linux) || defined(linux)
	cout << "Finished Sorting in " << (t2.tv_usec - t1.tv_usec) << "ms ..." << endl << endl;
#endif


	cout << endl << "Started Searching for Hull..." << endl;
		
#if defined(_WIN32) || defined(WIN32)
	QueryPerformanceCounter(&t3);
#elif defined(__linux) || defined(linux)
	gettimeofday(&t3, &tz);
#endif
	hull2D = hullFinder2D.FindHull(points);
#if defined(_WIN32) || defined(WIN32)
	QueryPerformanceCounter(&t4);
#elif defined(__linux) || defined(linux)
	gettimeofday(&t4, &tz);
#endif

#if defined(_WIN32) || defined(WIN32)
	cout << "Finished Searching for Hull in " << (t4.QuadPart - t3.QuadPart) * 1000.0 / frequency.QuadPart << "ms ..." << endl;
#elif defined(__linux) || defined(linux)
	cout << "Finished Searching for Hull in " << (t4.tv_usec - t3.tv_usec) << "ms ..." << endl;
#endif

	cout << endl;

#if defined(_WIN32) || defined(WIN32)
	cout << "Number of points: " << points->size() << ", Number of points in hull: " << hull2D->size() << ", Total time: " << (t2.QuadPart - t1.QuadPart + t4.QuadPart - t3.QuadPart) / 1000.0 << "ms" << endl;
#elif defined(__linux) || defined(linux)
	cout << "Number of points: " << points->size() << ", Number of points in hull: " << hull2D->size() << ", Total time: " << (t2.tv_usec - t1.tv_usec + t4.tv_usec - t3.tv_usec) << "ms" << endl;
#endif

	glutInit(&argc, argv);                 // Initialize GLUT
        if (!glutGet(GLUT_DISPLAY_MODE_POSSIBLE))
        { 
            exit(1); 
        }
	glutCreateWindow("2D Convex Hull Test"); // Create a window with the given title
	glutInitWindowSize(1024, 768);   // Set the window's initial width & height
	glutInitWindowPosition(250, 250); // Position the window's initial top-left corner
	glutDisplayFunc(Display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}

