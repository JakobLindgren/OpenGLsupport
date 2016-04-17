#include "DrawArea.h"
#ifdef WIN32
#include <Windows.h>
#endif

#include "Wrapper.h"
#include <GL/glu.h>

using namespace OpenGLsupport;

#include <iostream>

VertialDrawArea::VertialDrawArea(void)
{
	subHeight=0;
}

VertialDrawArea::~VertialDrawArea(void)
{
	std::cerr<<"~VertialDrawArea"<<std::endl;
	if (deleteChildren)
	{
		for (Drawable *d : drawables)
			delete d;
	}
}






void VertialDrawArea::addDrawable(Drawable* drawable)
{
	drawables.push_back(drawable);
}
void VertialDrawArea::removeDrawable(Drawable* drawable)
{
	for (unsigned int i=0;i<drawables.size();i++)
	{
		if (drawables[i] == drawable)
		{
			drawables.erase(drawables.begin() + i);
			return;
		}
	}
}

void VertialDrawArea::reshape(int width,int height)
{
	Drawable::reshape(width,height);
	if (drawables.size()>0)
	{
		subHeight = height / drawables.size();
		for (Drawable *d : drawables)
		{
			d->reshape(width,subHeight);
		}
	}
}

void VertialDrawArea::draw(void)
{
	if (drawables.size()>0)
	{
		GlPushMatrix pm;
		gluOrtho2D(-1, 1, -1, -1 + 2*drawables.size());
		drawables[0]->draw();
		for (unsigned int i=1;i<drawables.size();i++)
		{
			glTranslatef(0,2,0);
			drawables[i]->draw();
		}
	}
	else
	{
		glColor3f(1,0,0);
		GlBegin gb(GlBegin::lines);
		glVertex2i(+1,+1);
		glVertex2i(-1,-1);
		glVertex2i(-1,+1);
		glVertex2i(+1,-1);
	}
}
