#include "DrawArea.h"
#ifdef WIN32
#include <Windows.h>
#endif

#include "Wrapper.h"
#include <GL/glu.h>

using namespace OpenGLsupport;

#include <iostream>

DrawArea::DrawArea(void)
{
	borderWidth = 0;
	drawBorder = false;
}
void DrawArea::setBorderWidth(int width)
{
	borderWidth = width;
}
void DrawArea::setDrawBorder(bool value)
{
	drawBorder = value;
}
void DrawArea::drawThinBorder(void)
{
	GlBegin gb(GlBegin::line_loop);
	glVertex2i(+1,+1);
	glVertex2i(+1,-1);
	glVertex2i(-1,-1);
	glVertex2i(-1,+1);
}
void DrawArea::drawThinCross(void)
{
	GlBegin gb(GlBegin::lines);
	glVertex2i(+1,+1);
	glVertex2i(-1,-1);
	glVertex2i(-1,+1);
	glVertex2i(+1,-1);
}


VertialDrawArea::VertialDrawArea(void)
{
	subHeight=0;
	downToUp = true;
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
void VertialDrawArea::setDownToUp(bool value)
{
	downToUp = value;
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
	float scaleY = float(subHeight) / (subHeight+borderWidth*2);


	if (drawables.size()>0)
	{
		GlPushMatrix pm;
		gluOrtho2D(-1, 1, -1, -1 + 2*drawables.size());
		if (downToUp)
		{
			for (unsigned int i=0;i<drawables.size();i++)
			{
				if (drawBorder)
				{
					glColor3f(0,0,0);
					drawThinBorder();
				}
				{
					GlPushMatrix pm;
					glScalef(1,scaleY,1);
					drawables[i]->draw();
				}
				glTranslatef(0,2,0);
			}
		}
		else
		{
			unsigned int i = drawables.size();
			while (i--)
			{
				if (drawBorder)
				{
					glColor3f(0,0,0);
					drawThinBorder();
				}
				{
					GlPushMatrix pm;
					glScalef(1,scaleY,1);
					drawables[i]->draw();
				}
				glTranslatef(0,2,0);
			}
		}
	}
	else
	{
		glColor3f(1,0,0);
		drawThinCross();
		drawThinBorder();
	}
}



GridDrawArea::GridDrawArea(int cols,int rows):cols(cols),rows(rows)
{
	subHeight = 0;
	subWidth = 0;


	drawables = new Drawable**[cols];
	for (int x = 0;x<cols;x++)
	{
		drawables[x] = new Drawable*[rows];
		for (int y = 0;y<rows;y++)
		{
			drawables[x][y] = 0;
		}
	}
}


GridDrawArea::~GridDrawArea(void)
{
	std::cerr<<"~GridDrawArea"<<std::endl;
	if (deleteChildren)
	{
		for (int x = 0;x<cols;x++)
		{
			for (int y = 0;y<rows;y++)
			{
				if (drawables[x][y]) delete drawables[x][y];
			}
		}
	}

	for (int x = 0;x<cols;x++)
	{
		delete drawables[x];
	}
	delete drawables;

}
bool GridDrawArea::isValid(int x,int y)
{
	return ((0<=x) && (x<cols) && (0<=y) && (x<rows));
}

void GridDrawArea::addDrawable(int x,int y, Drawable* drawable)
{
	if (isValid(x,y))
	{
		drawables[x][y] = drawable;
		printf("GridDrawArea::addDrawable(%i,%i,%p)\n",x,y,drawable);
	}
}
void GridDrawArea::removeDrawable(int x,int y)
{
	if (isValid(x,y))
	{
		drawables[x][y] =  0;
	}
}

void GridDrawArea::reshape(int width,int height)
{
	Drawable::reshape(width,height);
	subHeight = height / rows;
	subWidth = width/ cols;


	for (int x = 0;x<cols;x++) for (int y = 0;y<rows;y++)
	{
		if (drawables[x][y]) drawables[x][y]->reshape(subWidth,subHeight);
	}
}



void GridDrawArea::draw(void)
{

	float scaleX = float(subWidth) / (subWidth+borderWidth*2);
	float scaleY = float(subHeight) / (subHeight+borderWidth*2);

	GlPushMatrix pm;
	gluOrtho2D(-1, -1  + 2*cols,-1, -1  + 2*rows);
	for (int x = 0;x<cols;x++)
	{
		{
			GlPushMatrix pm;
			for (int y = 0;y<rows;y++)
			{
				if (drawBorder)
				{
					glColor3f(0,0,0);
					drawThinBorder();
				}
				{
					GlPushMatrix pm;
					glScalef(scaleX,scaleY,1);
					if (drawables[x][y])
					{
						drawables[x][y]->draw();
					}
					else
					{
						glColor3f(1,0,0);
						drawThinCross();
						drawThinBorder();
					}
				}
				glTranslatef(0,2,0);
			}
		}
		glTranslatef(2,0,0);
	}
}


