#pragma once

#include <string>

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



	class GlColor
	{
	public:
		

  	static const GlColor Black;
  	static const GlColor White;
  	static const GlColor Red;
  	static const GlColor Lime;
  	static const GlColor Blue;
  	static const GlColor Yellow;
  	static const GlColor Cyan;
  	static const GlColor Magenta;
  	static const GlColor Silver;
  	static const GlColor Gray;
  	static const GlColor Maroon;
  	static const GlColor Olive;
  	static const GlColor Green;
  	static const GlColor Purple;
  	static const GlColor Teal;
  	static const GlColor Navy;

	private:
		union
		{
			struct
			{
				double r_d;
				double g_d;
				double b_d;
				double a_d;
			};
			double v_d[4];
		};

		union
		{
			struct
			{
				unsigned char r_ub;
				unsigned char g_ub;
				unsigned char b_ub;
				unsigned char a_ub;
			};
			unsigned char v_ub[4];
		};
		void toDouble(void);
		void toByte(void);
	public:
		GlColor(std::string html);
		GlColor(double r, double g, double b,double a=1);
		GlColor(double v[4]);
		GlColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
		GlColor(unsigned char v[4]);
		void set(void) const;
		void get(double &r, double &g, double &b) const;
		void get(double &r, double &g, double &b, double &a) const;
		void get(unsigned char &r, unsigned char &g, unsigned char &b) const;
		void get(unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a) const;

		double const* get_dv(void) const;
		unsigned char const* get_ubv(void) const;
	};

}
