#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <stdio.h>
#include <iostream>
#define _USE_MATH_DEFINES // for C++
#include <math.h>
#include <random>
#include <algorithm>
#include <vector>

#include "Point.h"
#include "HeapSort.h"
#include "Hull2D.h"
#include "Hull3D.h"

using namespace std;


vector<Point*>* points = new vector<Point*>();
vector<Point*>* hull2D;
vector<Point*>* hull3D;

bool mouseDown = false;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

//Handler for window-repaint event. Call back when the window first appears and
//whenever the window needs to be re-painted.
bool Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	return true;
}

void Display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);	// Red
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);	// Red
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);	// Red
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f);	// Red
	for (unsigned int i = 0; i < points->size(); i++)
	{
		float intensity = (float)(i + 1) / points->size();
		glColor3f(intensity, intensity, intensity);	// Red
		glVertex3f((*points)[i]->X - 0.01f, (*points)[i]->Y - 0.01f, (*points)[i]->Z);    // x, y, z
		glVertex3f((*points)[i]->X + 0.01f, (*points)[i]->Y - 0.01f, (*points)[i]->Z);    // x, y, z
		glVertex3f((*points)[i]->X + 0.01f, (*points)[i]->Y + 0.01f, (*points)[i]->Z);    // x, y, z
		glVertex3f((*points)[i]->X - 0.01f, (*points)[i]->Y + 0.01f, (*points)[i]->Z);    // x, y, z
	}
	glEnd();

	
/*	glBegin(GL_POLYGON);
	glColor3f(0.0f, 1.0f, 0.0f); // Green
	for (unsigned int i = 0; i < hull2D->size(); i++)
	{
		glVertex3f((*hull2D)[i]->X, (*hull2D)[i]->Y, (*hull2D)[i]->Z);    // x, y, z
	}
	glEnd();*/

	int numTriangles = 0;
	

	vector<bool> lookedAtIndex (points->size());
	fill(lookedAtIndex.begin(), lookedAtIndex.end(), false);

	glColor3f(0.5f, 0.5f, 1.0f); // Light Blue
	for (unsigned int i = 0; i < hull3D->size(); i++)
	{
		int numTrianglesTemp = 0;
		lookedAtIndex[(*hull3D)[i]->Index] = true;
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f((*hull3D)[i]->X, (*hull3D)[i]->Y, (*hull3D)[i]->Z);    // x, y, z
		numTrianglesTemp++;
		for (unsigned int j = 0; j < (*hull3D)[i]->neighbours.size(); j++)
		{
			if (lookedAtIndex[(*hull3D)[i]->neighbours[j]] == true)
				continue;

			glVertex3f((*points)[(*hull3D)[i]->neighbours[j]]->X, (*points)[(*hull3D)[i]->neighbours[j]]->Y, (*points)[(*hull3D)[i]->neighbours[j]]->Z);    // x, y, z
			numTrianglesTemp++;
		}
		if ((*hull3D)[i]->neighbours.size() > 1 && lookedAtIndex[(*hull3D)[i]->neighbours[0]] == false)
		{
			glVertex3f((*points)[(*hull3D)[i]->neighbours[0]]->X, (*points)[(*hull3D)[i]->neighbours[0]]->Y, (*points)[(*hull3D)[i]->neighbours[0]]->Z);    // x, y, z
			numTrianglesTemp++;
		}

		if (numTrianglesTemp > 2)
			numTriangles += (numTrianglesTemp - 2);

		glEnd();
	}
	
	for (unsigned int i = 0; i < hull3D->size(); i++)
	{
		for (unsigned int j = 0; j < (*hull3D)[i]->neighbours.size(); j++)
		{
			if (j > 0)
			{
				glBegin(GL_LINES);
				Vec3D firstLeg = Vec3D((Vec3D)*(*hull3D)[i], (Vec3D)*(*points)[(*hull3D)[i]->neighbours[j-1]]);
				Vec3D secondLeg = Vec3D((Vec3D)*(*hull3D)[i], (Vec3D)*(*points)[(*hull3D)[i]->neighbours[j]]);
				Vec3D crossLeg = firstLeg.NormCrossP(secondLeg);
				Vec3D midpoint = Vec3D((*hull3D)[i]->X + firstLeg.X / 4 + secondLeg.X / 4,
					(*hull3D)[i]->Y + firstLeg.Y / 4 + secondLeg.Y / 4,
					(*hull3D)[i]->Z + firstLeg.Z / 4 + secondLeg.Z / 4);

				glVertex3f(midpoint.X, midpoint.Y, midpoint.Z);
				glVertex3f(midpoint.X + crossLeg.X / 5, midpoint.Y + crossLeg.Y / 5, midpoint.Z + crossLeg.Z / 5);
				glEnd();
			}
		}
	}/**/


	cout << "Number of faces: " << numTriangles << endl;

	glFlush();  // Render now
	glutSwapBuffers();
}


void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;

		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	else
		mouseDown = false;
}

void MouseMotion(int x, int y)
{
	if (mouseDown)
	{
		yrot = x - xdiff;
		xrot = y + ydiff;

		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	srand(5);
	HeapSort sorter = HeapSort();
	Hull2D hullFinder2D = Hull2D();
	Hull3D hullFinder3D = Hull3D();

	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER tStart, tEnd;           // ticks
	double elapsedTime = 0;
	double totalTime = 0;
	
	QueryPerformanceFrequency(&frequency);

	/*
	for (unsigned int i = 0; i < 8; i++)
	{
		double theta = (float)rand() / RAND_MAX * M_PI * 2;
		double phi = (float)rand() / RAND_MAX * M_PI * 2;
		double r = (float)rand() / RAND_MAX * 0.5f;
		points->push_back(new Point(sin(theta) * cos(phi) * r, sin(theta) * sin(phi) * r, cos(theta) * r, points->size()));
	}
	*/

//	points->push_back(new Point(-0.15f, -0.15f, -0.15f, 0));
//	points->push_back(new Point(0.15f, -0.15f, -0.15f, 1));
//	points->push_back(new Point(0.0f, -0.15f, 0.15f, 2));
//	points->push_back(new Point(0.01f, 0.15f, 0.0f, 3));

//	points->push_back(new Point(0.25f, 0.25f, 0.25f, 4));
//	points->push_back(new Point(0.55f, 0.25f, 0.25f, 5));
//	points->push_back(new Point(0.4f, 0.25f, 0.55f, 6));
//	points->push_back(new Point(0.41f, 0.55f, 0.4f, 7));

	points->push_back(new Point(-0.3f, -0.3f, 0.0f, 0));
	points->push_back(new Point(-0.3f, 0.3f, 0.0f, 1));

	points->push_back(new Point(0.3f, 0.0f, -0.3f, 2));
	points->push_back(new Point(0.3f, 0.0f, 0.3f, 3));

	cout << endl;

	cout << "Started Sorting..." << endl;

	QueryPerformanceCounter(&tStart);
	sorter.Sort(points);
	for (unsigned int i = 0; i < points->size(); ++i)
		(*points)[i]->Index = i;
	QueryPerformanceCounter(&tEnd);
		
	totalTime += (elapsedTime = (tEnd.QuadPart - tStart.QuadPart) * 1000.0 / frequency.QuadPart);
	cout << "Finished Sorting in " << elapsedTime << "ms ..." << endl << endl;
	cout << endl;
	cout << "Started Searching for 2D Hull..." << endl;
		
	QueryPerformanceCounter(&tStart);
	hull2D = hullFinder2D.FindHull(points);
	QueryPerformanceCounter(&tEnd);

	totalTime += (elapsedTime = (tEnd.QuadPart - tStart.QuadPart) * 1000.0 / frequency.QuadPart);
	cout << "Finished Searching for 2D Hull in " << elapsedTime << "ms ..." << endl;
	cout << endl;
	cout << "Started Searching for 3D Hull..." << endl;

	QueryPerformanceCounter(&tStart);
	hull3D = hullFinder3D.FindHull(points);
	QueryPerformanceCounter(&tEnd);

	totalTime += (elapsedTime = (tEnd.QuadPart - tStart.QuadPart) * 1000.0 / frequency.QuadPart);
	cout << "Finished Searching for 3D Hull in " << elapsedTime << "ms ..." << endl;
	cout << endl;
	cout << "Total number of points: " << points->size() << endl;
	cout << "Number of points in 2D Hull: " << hull2D->size() << endl;
	cout << "Number of points in 3D Hull: " << hull3D->size() << endl;
	cout << "Total time: " << totalTime << "ms" << endl;


	glutInit(&argc, argv);					// Initialize GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1024, 768);			// Set the window's initial width & height
	glutInitWindowPosition(250, 250);		// Position the window's initial top-left corner
	glutCreateWindow("3D Convex Hull");		// Create a window with the given title
	glutDisplayFunc(Display);				// Register display callback handler for window re-paint
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

	if (!Init())
		return 1;

	glutMainLoop();							// Enter the infinitely event-processing loop

	return 0;
}