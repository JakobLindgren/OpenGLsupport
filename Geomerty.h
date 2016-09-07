
#pragma once

#include <vector>
#include "Wrapper.h"
#include "Drawable.h"

namespace OpenGLsupport
{

	class GeometryBase
	{
	public:
		union Point
		{
			struct
			{
				double x, y;
			};
			double v[2];
		};
	public:
		virtual ~GeometryBase(void);
		virtual void draw(void) = 0;

	};

	class GeometryPointVector :public GeometryBase
	{
	protected:

		std::vector<Point> bufferPoints[2];
		std::vector<Point> *readPoints, *writePoints;
	public:
		GeometryPointVector(void);
		void add(double x, double y);
		void add(GeometryBase::Point p);
		void add(double *x, double *y, int length);
		void add(GeometryBase::Point *p, int length);
		void add(std::vector<GeometryBase::Point> &p);

		void swap(void);
	};

	class GeometryPointCloud :public GeometryPointVector
	{
	private:
		GlColor pointColor;
	public:
		GeometryPointCloud(void);
		void draw(void) override;
	};

	class GeometryLine :public GeometryPointVector
	{
	private:
		GlColor lineColor;
	public:
		GeometryLine(void);
		void draw(void) override;
	};

	class GeometryShape :public GeometryPointVector
	{
	private:
		GlColor fillColor;
		GlColor lineColor;
	public:
		GeometryShape(void);
		void draw(void) override;
	};

	class GeometryContainer :public Drawable
	{
	private:
		std::vector<GeometryBase*> shapes;
	public:
		GeometryContainer(void);
		void add(GeometryBase *s);
		void draw(void) override;
	};
}