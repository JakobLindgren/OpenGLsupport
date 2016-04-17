#include "Drawable.h"

using namespace OpenGLsupport;

Drawable::~Drawable(void){}

void Drawable::reshape(int width,int height)
{
	this->width = width;
	this->height = height;
}


DrawableContainer::DrawableContainer(bool deleteChildren):deleteChildren(deleteChildren){}
DrawableContainer::~DrawableContainer(void){}
void DrawableContainer::setDeleteChildren(bool value)
{
	deleteChildren = value;
}
