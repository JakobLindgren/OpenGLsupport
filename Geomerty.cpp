#include "Geomerty.h"

#include <GL\GLU.h>

using namespace OpenGLsupport;

GeometryBase::~GeometryBase(void) {}

GeometryPointVector::GeometryPointVector(void)
{
	readPoints = bufferPoints + 0;
	writePoints = bufferPoints + 1;
}

void GeometryPointVector::add(double x, double y)
{
	writePoints->push_back({ x,y });
}
void GeometryPointVector::add(GeometryBase::Point p)
{
	writePoints->push_back(p);
}
void GeometryPointVector::add(std::vector<GeometryBase::Point> &p)
{
	writePoints->insert(writePoints->end(), p.begin(), p.end());
}
void GeometryPointVector::add(double *x, double *y, int length)
{
	while (length--)
	{
		add(*(x++), *(y++));
	}
}
void GeometryPointVector::add(GeometryBase::Point *p, int length)
{
	writePoints->insert(writePoints->end(), p, p + length);
}

void GeometryPointVector::swap(void)
{
	std::vector<Point> *temp = readPoints;
	readPoints = writePoints;
	writePoints = temp;
	writePoints->clear();
}


GeometryPointCloud::GeometryPointCloud(void) :pointColor(GlColor::Black)
{}

void GeometryPointCloud::draw(void)
{
	pointColor.set();
	{
		GlBegin gb(GlBegin::points);
		for (GeometryBase::Point &p : *readPoints)
		{
			glVertex2dv(p.v);
		}
	}
}

GeometryLine::GeometryLine(void) :lineColor(GlColor::Black)
{}

void GeometryLine::draw(void)
{
	lineColor.set();
	{
		GlBegin gb(GlBegin::line_strip);
		for (GeometryBase::Point &p : *readPoints)
		{
			glVertex2dv(p.v);
		}
	}
}


GeometryShape::GeometryShape(void) :fillColor(GlColor::Yellow), lineColor(GlColor::Black)
{}

void GeometryShape::draw(void)
{
	fillColor.set();
	{
		GlBegin gb(GlBegin::triangle_fan);
		for (GeometryBase::Point &p : *readPoints)
		{
			glVertex2dv(p.v);
		}
	}
	lineColor.set();
	{
		GlBegin gb(GlBegin::line_loop);
		for (GeometryBase::Point &p : *readPoints)
		{
			glVertex2dv(p.v);
		}
	}
}


GeometryContainer::GeometryContainer(void) {}
void GeometryContainer::add(GeometryBase *s)
{
	shapes.push_back(s);
}
void GeometryContainer::draw(void)
{
	GlPushMatrix gpm();
	gluOrtho2D(-10, 10, -10, 10);
	for (GeometryBase *s : shapes) s->draw();
}
