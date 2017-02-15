/*
 * ColorPalette.cpp
 *
 *  Created on: Sep 21, 2016
 *      Author: jalin
 */

#include "ColorPalette.h"
#include <algorithm>

using namespace OpenGLsupport;


ColorPalette::ColorLimit::ColorLimit(float value,GlColor &color):GlColor(color),value(value){}
ColorPalette::ColorLimit::ColorLimit(float value,std::string html):GlColor(html),value(value){}
ColorPalette::ColorLimit::ColorLimit(float value,double r, double g, double b,double a):GlColor(r,g,b,a),value(value){}
ColorPalette::ColorLimit::ColorLimit(float value,double v[4]):GlColor(v),value(value){}
ColorPalette::ColorLimit::ColorLimit(float value,unsigned char r, unsigned char g, unsigned char b, unsigned char a):GlColor(r,g,b,a),value(value){}
ColorPalette::ColorLimit::ColorLimit(float value,unsigned char v[4]):GlColor(v),value(value){}
bool ColorPalette::ColorLimit::operator< (const ColorLimit& other) const
{
    return (value < other.value);
}


ColorPalette::ColorPalette()
{
	limitLength = 0;
	FirstColorLimit = 0;
	LastColorLimit = 0;
}

void ColorPalette::addColor(float value,float color[3])
{
	addColor(value,color[0],color[1],color[2]);
}
void ColorPalette::addColor(float value,float R,float G,float B)
{
	colorLimits.push_back(ColorLimit(value,R,G,B));
	limitLength++;
}
void ColorPalette::addColor(float value,GlColor &color)
{
	colorLimits.push_back(ColorLimit(value,color));
	limitLength++;
}


void ColorPalette::generate(void)
{
	std::sort(colorLimits.begin(), colorLimits.end());
	ColorLimit const * ColorLimitArray = colorLimits.data();

	FirstColorLimit = ColorLimitArray;
	LastColorLimit = ColorLimitArray + limitLength-1;

}

void LinearColorPalette::getColor(float value,float color[3])
{
	if (value<=FirstColorLimit->value)
	{
		for (int i=0;i<3;i++) color[i] = FirstColorLimit->get_dv()[i];
		return;
	}
	if (value>=LastColorLimit->value)
	{
		for (int i=0;i<3;i++) color[i] = LastColorLimit->get_dv()[i];
		return;
	}

	int cnt = limitLength-1;

	ColorLimit const * CurrentColorLimit = FirstColorLimit;

	while(cnt--)
	{
		if ((CurrentColorLimit[0].value <= value) && (value<CurrentColorLimit[1].value))
		{
			float k2 = (value - CurrentColorLimit[0].value) / (CurrentColorLimit[1].value - CurrentColorLimit[0].value);
			float k1 = 1-k2;

			double const * const color1 = CurrentColorLimit[0].get_dv();
			double const * const color2 = CurrentColorLimit[1].get_dv();

			for (int i=0;i<3;i++)
			{
				color[i] = color1[i]*k1 + color2[i]*k2;
			}
			return;
		}
		CurrentColorLimit++;
	}
};
