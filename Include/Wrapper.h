#pragma once

#include <string>
#include "Constants.h"
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
	public:
		GlBegin(DrawMode mode);
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
		GlColor(const GlColor& other);
		GlColor(std::string html);
		GlColor(double r, double g, double b,double a=1);
		GlColor(double *v,int length);
		GlColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
		GlColor(unsigned char  *v, int length);
		void set(void) const;
		void get(double &r, double &g, double &b) const;
		void get(double &r, double &g, double &b, double &a) const;
		void get(unsigned char &r, unsigned char &g, unsigned char &b) const;
		void get(unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a) const;

		double const*const get_dv(void) const;
		unsigned char const*const get_ubv(void) const;
	};

}
