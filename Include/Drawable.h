#pragma once

namespace OpenGLsupport
{
	class Drawable
	{
	protected:
		int width;
		int height;
	public:
		virtual ~Drawable(void);
		virtual void draw(void) = 0;
		virtual void reshape(int width,int height);
	};

	class DrawableContainer
	{
	protected:
		bool deleteChildren;
	public:
		DrawableContainer(bool deleteChildren = false);
		virtual ~DrawableContainer(void);
		void setDeleteChildren(bool value);
	};
}
