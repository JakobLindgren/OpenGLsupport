#pragma once

namespace OpenGLsupport
{
	enum DrawMode
	{
		points = 0,
		lines = 1,
		line_loop = 2,
		line_strip = 3,
		triangles = 4,
		triangle_strip = 5,
		triangle_fan = 6,
		quads = 7,
		quad_strip = 8,
		polygon = 9,
	};

#ifndef NDEBUG
	bool validateConstants(void);
#endif
}