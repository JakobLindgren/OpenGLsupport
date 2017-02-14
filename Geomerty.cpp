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
void GeometryPointVector::add(std::vector<double> &x, std::vector<double> &y)
{
	int lenX = x.size();
	int lenY = y.size();

	double *X = x.data();
	double *Y = y.data();

	while((lenX--) && (lenY--))
	{
		add(*(X++), *(Y++));
	}

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
		GlBegin gb(DrawMode::points);
		for (GeometryBase::Point &p : *readPoints)
		{
			glVertex2dv(p.v);
		}
	}
}

GeometryLine::GeometryLine(void) :lineColor(GlColor::Black),mode(DrawMode::lines)
{}
void GeometryLine::setMode(DrawMode value)
{
	mode = value;
}

void GeometryLine::draw(void)
{
	lineColor.set();
	{
		GlBegin gb(mode);
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
		GlBegin gb(DrawMode::triangle_fan);
		for (GeometryBase::Point &p : *readPoints)
		{
			glVertex2dv(p.v);
		}
	}
	lineColor.set();
	{
		GlBegin gb(DrawMode::line_loop);
		for (GeometryBase::Point &p : *readPoints)
		{
			glVertex2dv(p.v);
		}
	}
}

GeometryContainer::GeometryContainer(void)
{
	limits[0] = -10;
	limits[1] = 10;
	limits[2] = -10;
	limits[3] = 10;
}
void GeometryContainer::setLimits(double *limits)
{
	for (int i = 0; i < 4; i++)
	{
		this->limits[i] = limits[i];
	}
}

void GeometryContainer::add(GeometryBase *s)
{
	shapes.push_back(s);
}
void GeometryContainer::clear(bool deleteContent)
{
	for (GeometryBase *s : shapes)
	{
		delete s;
	}
	shapes.clear();
}
void GeometryContainer::draw(void)
{
	GlPushMatrix gpm;
	gluOrtho2D(limits[0], limits[1], limits[2], limits[3]);
	for (GeometryBase *s : shapes) s->draw();
}
