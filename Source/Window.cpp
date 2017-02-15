#include "../Include/Window.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "../Include/Wrapper.h"
#include <GL/freeglut.h>
#include <iostream>

using namespace OpenGLsupport;

std::vector<Window*> WindowManager::startingWindows;
std::map<int, Window*> WindowManager::activeWindows;
std::vector<Window*> WindowManager::endingWindows;


std::mutex WindowManager::Windowlock;
std::thread* WindowManager::mainloop = 0;
bool WindowManager::runMainLoop = true;

void WindowManager::start(Window *window)
{
	Windowlock.lock();
	startingWindows.push_back(window);
	Windowlock.unlock();
}
void WindowManager::end(Window *window)
{
	Windowlock.lock();
	endingWindows.push_back(window);
	Windowlock.unlock();
	while (window->window) Sleep(100);
}





void WindowManager::init(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
}
void WindowManager::mainLoop(bool inThread)
{
	runMainLoop = true;
	if (inThread)
	{
		if (mainloop == 0)
		{
			mainloop = new std::thread(&WindowManager::mainLoop,false);
		}
	}
	else
	{
		while (runMainLoop)
		{
			if (startingWindows.size())
			{
				Windowlock.lock();
				for (std::vector<Window*>::iterator itr = startingWindows.begin(); itr != startingWindows.end(); itr++)
				{
					Window *wPtr = *itr;
					std::string windowName = wPtr->name;
					int windowIndex = glutCreateWindow(windowName.c_str());

					wPtr->window = windowIndex;
					activeWindows[windowIndex] = wPtr;
					glClearColor(1, 1, 1, 1);
					glutDisplayFunc(WindowManager::staticDraw);
					glutReshapeFunc(WindowManager::staticReshape);
				}
				startingWindows.clear();
				Windowlock.unlock();
			}
			if (endingWindows.size())
			{
				Windowlock.lock();
				for (auto w = endingWindows.begin(); w != endingWindows.end(); w++)
				{
					activeWindows.erase((*w)->window);
					(*w)->window = 0;
				}
				endingWindows.clear();
				Windowlock.unlock();
			}
			checkRedraw();
			if (activeWindows.size())
			{
				glutMainLoopEvent();
			}
			else
			{
				Sleep(100);
			}
		}
	}
}

void OpenGLsupport::WindowManager::stopMainLoop(void)
{
	runMainLoop = false;
}

void WindowManager::staticDraw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	activeWindows[glutGetWindow()]->draw();
	glutSwapBuffers();
}

void WindowManager::staticReshape(int w, int h)
{
	activeWindows[glutGetWindow()]->reshape(w,h);
}

void WindowManager::checkRedraw(void)
{
	for (std::map<int, Window*>::iterator itr = activeWindows.begin();itr != activeWindows.end();itr++)
	{
		itr->second->checkRedraw();
	}
}





Window::Window(std::string name,Drawable *drawable):drawable(drawable), name(name)
{
	width = 1;
	height = 1;
	lastRedraw=0;
	currentRedraw=0;

	//window = glutCreateWindow(name.c_str());
	window = 0;




	WindowManager::start(this);
}
Window::~Window(void)
{
	std::cerr<<"~Window"<<std::endl;
	WindowManager::end(this);
	if (deleteChildren)
	{
		delete drawable;
	}
}
void Window::setDrawable(Drawable *drawable)
{
	this->drawable = drawable;
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
		GlBegin gb(DrawMode::lines);
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
