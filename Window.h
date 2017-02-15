#pragma once
#include <map>
#include <mutex>
#include <thread>
#include <string>

#include "Drawable.h"
namespace OpenGLsupport
{

	class Window:public DrawableContainer
	{
	private:
		static std::map<int, Window*> ActiveWindows;
		static std::mutex Windowlock;
		static std::thread *mainloop;
		static void staticIdle(void);
		static void staticTimer(int value);
		static void staticDraw(void);
		static void staticReshape(int w, int h);
	public:
		static void init(int argv, char **argc);
		static void mainLoop(bool inThread=true);


	private:
		//bool redisplayFlag;
		unsigned int lastRedraw;
		unsigned int currentRedraw;
		void checkRedraw(void);
		//std::string name;
		int window;
		Drawable *drawable;
	protected:
		int width;
		int height;
	public:
		Window(std::string name,Drawable *drawable = 0);
		virtual ~Window(void);
		virtual void draw(void);
		virtual void reshape(int w, int h);
		void redisplay(void);
		void getLocation(int &x,int &y);
		void setLocation(int x,int y);
		void getSize(int &w,int &h);
		void setSize(int w,int h);
	};
}
