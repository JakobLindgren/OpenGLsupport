#pragma once
#include <vector>
#include "Wrapper.h"
namespace OpenGLsupport
{

class ColorPalette
{
protected:

	class ColorLimit:public GlColor
	{
	public:
		float value;
	
		ColorLimit(float value,GlColor &c);
		ColorLimit(float value,std::string html);
		ColorLimit(float value,double r, double g, double b,double a=1);
		ColorLimit(float value,double v[4]);
		ColorLimit(float value,unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
		ColorLimit(float value,unsigned char v[4]);
		bool operator< (const ColorLimit& other) const;
	};

	//std::vector<float> values;
	//std::vector<GlColor> colors;

	std::vector<ColorLimit> colorLimits;

	ColorLimit const * FirstColorLimit;
	ColorLimit const * LastColorLimit;
	int limitLength;
public:
	ColorPalette();
	virtual ~ColorPalette(){}

	void addColor(float value,GlColor &color);
	void addColor(float value,float color[3]);
	void addColor(float value,float R,float G,float B);
	void generate(void);

	virtual void getColor(float value,float color[3])=0;

};

class LinearColorPalette:public ColorPalette
{
public:
	void getColor(float value,float color[3]) override;
};

}

