#include "Window.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "Wrapper.h"
#include <GL/freeglut.h>
#include <iostream>

using namespace OpenGLsupport;

std::map<int, Window*> Window::ActiveWindows;
std::mutex Window::Windowlock;
std::thread* Window::mainloop = 0;

void Window::init(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 300);
	glutIdleFunc(Window::staticIdle);
}
void Window::mainLoop(bool inThread)
{
	if (inThread)
	{
		if (mainloop == 0)
		{
			mainloop = new std::thread(glutMainLoop);
		}
	}
	else
	{
		glutMainLoop();
	}
}

void Window::staticDraw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Windowlock.lock();
	ActiveWindows[glutGetWindow()]->draw();
	Windowlock.unlock();
	glutSwapBuffers();
}

void Window::staticReshape(int w, int h)
{
	Windowlock.lock();
	ActiveWindows[glutGetWindow()]->reshape(w,h);
	Windowlock.unlock();
}

void Window::staticIdle(void)
{
	Windowlock.lock();
	for (std::map<int, Window*>::iterator itr = ActiveWindows.begin();itr != ActiveWindows.end();itr++)
	{
		itr->second->checkRedraw();
	}
	Windowlock.unlock();
}





Window::Window(std::string name,Drawable *drawable):drawable(drawable)
{
	width = 1;
	height = 1;
	lastRedraw=0;
	currentRedraw=0;

	window = glutCreateWindow(name.c_str());

	Windowlock.lock();
	ActiveWindows[window] = this;
	Windowlock.unlock();

	glClearColor(1, 1, 1, 1);
	glutDisplayFunc(Window::staticDraw);
	glutReshapeFunc(Window::staticReshape);
}
Window::~Window(void)
{
	std::cerr<<"~Window"<<std::endl;
	Windowlock.lock();
	ActiveWindows.erase(window);
	Windowlock.unlock();
	glutDestroyWindow(window);

	if (deleteChildren)
	{
		delete drawable;
	}
}

void Window::reshape(int w, int h)
{
	width = w;
	height = h;

	if (drawable) drawable->reshape(w,h);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}
void Window::draw(void)
{
	if (drawable)
		drawable->draw();
	else
	{
		glColor3f(0,0,0);
		GlBegin gb(GlBegin::lines);
		glVertex2i(+1,+1);
		glVertex2i(-1,-1);
		glVertex2i(-1,+1);
		glVertex2i(+1,-1);
	}
}
void Window::redisplay(void)
{
	currentRedraw++;
}
void Window::checkRedraw(void)
{
	if (lastRedraw != currentRedraw)
	{
		lastRedraw = currentRedraw;
		glutSetWindow(window);
		glutPostRedisplay();
	}
}

void Window::getLocation(int &x,int &y)
{
	glutSetWindow(window);
	x = glutGet((GLenum)GLUT_WINDOW_X);
	y = glutGet((GLenum)GLUT_WINDOW_Y);
}
void Window::setLocation(int x,int y)
{
	glutSetWindow(window);
	glutPositionWindow(x,y);
}
void Window::getSize(int &w,int &h)
{
	w = width;
	h = height;
}
void Window::setSize(int w,int h)
{
	glutSetWindow(window);
	glutReshapeWindow(w,h);
}
