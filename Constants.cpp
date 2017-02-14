#include "Constants.h"
#ifdef WIN32
#include <Windows.h>
#endif

#include <GL\GL.h>
bool OpenGLsupport::validateConstants(void)
{
if (points != GL_POINTS) return false;
if (lines != GL_LINES) return false;
if (line_loop != GL_LINE_LOOP) return false;
if (line_strip != GL_LINE_STRIP) return false;
if (triangles != GL_TRIANGLES) return false;
if (triangle_strip != GL_TRIANGLE_STRIP) return false;
if (triangle_fan != GL_TRIANGLE_FAN) return false;
if (quads != GL_QUADS) return false;
if (quad_strip != GL_QUAD_STRIP) return false;
if (polygon != GL_POLYGON) return false;

	return true;
}
