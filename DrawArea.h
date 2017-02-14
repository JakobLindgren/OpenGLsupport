#pragma once
#include <string>
#include <vector>
#include "Drawable.h"



namespace OpenGLsupport
{

class DrawArea:public Drawable,public DrawableContainer
{
protected:
	int borderWidth;
	bool drawBorder;

	void drawThinBorder(void);
	void drawThinCross(void);
public:
	DrawArea(void);
	virtual ~DrawArea(void){}
	void setBorderWidth(int width);
	void setDrawBorder(bool value);
};


class VertialDrawArea:public DrawArea
{
private:
	std::vector<Drawable*> drawables;
	int subHeight;
	bool downToUp;
public:
	VertialDrawArea(void);
	~VertialDrawArea(void);
	void setDownToUp(bool value);
	void addDrawable(Drawable*);
	void removeDrawable(Drawable*);
	void draw(void) override;
	virtual void reshape(int width,int height);
};

class GridDrawArea:public DrawArea
{
private:
	Drawable ***drawables; //2d array of pointers
	int cols;
	int rows;

	int subHeight;
	int subWidth;
	//bool downToUp;
	bool isValid(int x,int y);
public:
	GridDrawArea(int cols,int rows);
	~GridDrawArea(void);
	void addDrawable(int x,int y, Drawable*);
	void removeDrawable(int x,int y);
	void draw(void) override;
	virtual void reshape(int width,int height);
};


}
