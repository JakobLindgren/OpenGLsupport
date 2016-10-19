#include "Wrapper.h"
using namespace OpenGLsupport;

GlBegin::GlBegin(Mode mode)
{
	glBegin(mode);
}
GlBegin::~GlBegin(void)
{
	glEnd();
}

GlPushMatrix::GlPushMatrix(void)
{
	glPushMatrix();
}
GlPushMatrix::~GlPushMatrix(void)
{
	glPopMatrix();
}



const GlColor GlColor::Black("000000");
const GlColor GlColor::White("FFFFFF");
const GlColor GlColor::Red("FF0000");
const GlColor GlColor::Lime("00FF00");
const GlColor GlColor::Blue("0000FF");
const GlColor GlColor::Yellow("FFFF00");
const GlColor GlColor::Cyan("00FFFF");
const GlColor GlColor::Magenta("FF00FF");
const GlColor GlColor::Silver("C0C0C0");
const GlColor GlColor::Gray("808080");
const GlColor GlColor::Maroon("800000");
const GlColor GlColor::Olive("808000");
const GlColor GlColor::Green("008000");
const GlColor GlColor::Purple("800080");
const GlColor GlColor::Teal("008080");
const GlColor GlColor::Navy("000080");

void GlColor::toDouble(void)
{
	for (int i = 0; i < 4; i++)
	{
		v_d[i] = v_ub[i] / 255.0;
	}
}
void GlColor::toByte(void)
{
	for (int i = 0; i < 4; i++)
	{
		v_ub[i] = v_d[i] * 255;
	}
}

GlColor::GlColor(const GlColor& other)
{
	v_d[0] = other.v_d[0];
	v_d[1] = other.v_d[1];
	v_d[2] = other.v_d[2];
	v_d[3] = other.v_d[3];

	v_ub[0] = other.v_ub[0];
	v_ub[1] = other.v_ub[1];
	v_ub[2] = other.v_ub[2];
	v_ub[3] = other.v_ub[3];
}
GlColor::GlColor(std::string html)
{
	for (int i = 0; i < 4; i++)
		v_ub[i] = 255;

	for (unsigned int i = 0; i < html.length(); i+=2)
	{
		unsigned char data = 0;
		for (int j = 0; j < 2; j++)
		{
			char read = html[i + j];
			data <<= 4;
			if (('0' <= read) && (read <= '9'))
			{
				data += read - '0';
			}
			else if (('A' <= read) && (read <= 'F'))
			{
				data += read - 'A' + 10;
			}
			else if (('a' <= read) && (read <= 'f'))
			{
				data += read - 'a' + 10;
			}
		}

		v_ub[i/2] = data;
	}
	toDouble();
}

GlColor::GlColor(double v[4]) :GlColor(v[0], v[1], v[2], v[3]) {}
GlColor::GlColor(unsigned char v[4]) : GlColor(v[0], v[1], v[2], v[3]) {}


GlColor::GlColor(double r, double g, double b, double a)
{
	r_d  = r;
	g_d  = g;
	b_d  = b;
	a_d  = a;

	toByte();
}
GlColor::GlColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	r_ub = r;
	g_ub = g;
	b_ub = b;
	a_ub = a;

	toDouble();
}
void GlColor::set(void) const
{
	glColor3dv(v_d);
}
void GlColor::get(double &r, double &g, double &b) const
{
	r = r_d;
	g = g_d;
	b = b_d;
}
void GlColor::get(double &r, double &g, double &b, double &a) const
{
	r = r_d;
	g = g_d;
	b = b_d;
	a = a_d;
}
void GlColor::get(unsigned char &r, unsigned char &g, unsigned char &b) const
{
	r = r_ub;
	g = g_ub;
	b = b_ub;
}
void GlColor::get(unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a) const
{
	r = r_ub;
	g = g_ub;
	b = b_ub;
	a = a_ub;
}

double const*const GlColor::get_dv(void) const
{
	return v_d;
}
unsigned char const*const GlColor::get_ubv(void) const
{
	return v_ub;
}
