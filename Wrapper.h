#ifdef WIN32
#include <Windows.h>
#include <GL/GL.h>
#endif


#ifdef __linux__
#include <GL/gl.h>
#endif


namespace OpenGLsupport
{
	class GlBegin
	{
	public:
		enum Mode {
			points = GL_POINTS,
			lines = GL_LINES,
			line_strip = GL_LINE_STRIP,
			line_loop = GL_LINE_LOOP,
			triangles = GL_TRIANGLES,
			triangle_strip = GL_TRIANGLE_STRIP,
			triangle_fan = GL_TRIANGLE_FAN,
			quads = GL_QUADS,
			quad_strip = GL_QUAD_STRIP,
			polygon = GL_POLYGON,
		};
	public:
		GlBegin(Mode mode);
		~GlBegin(void);
	};

	class GlPushMatrix
	{
	public:
		GlPushMatrix(void);
		~GlPushMatrix(void);
	};
}
