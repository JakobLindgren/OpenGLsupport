#pragma once
#include <string>
#include "Drawable.h"
#include <vector>
namespace OpenGLsupport
{

class DrawArea:public Drawable,public DrawableContainer
{
public:
	virtual ~DrawArea(void){}
};


class VertialDrawArea:public DrawArea
{
private:
	std::vector<Drawable*> drawables;
	int subHeight;
public:
	VertialDrawArea(void);
	~VertialDrawArea(void);
	void addDrawable(Drawable*);
	void removeDrawable(Drawable*);
	void draw(void) override;
	virtual void reshape(int width,int height);
};

}
