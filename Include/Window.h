#pragma once
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <string>

#include "Drawable.h"
namespace OpenGLsupport
{
	class Window;
	class WindowManager
	{
	private:
		static std::vector<Window*> startingWindows;
		static std::map<int, Window*> activeWindows;
		static std::vector<Window*> endingWindows;
		static std::mutex Windowlock;
		static std::thread *mainloop;
		static bool runMainLoop;

		//static void staticIdle(void);
		static void staticDraw(void);
		static void staticReshape(int w, int h);
		static void checkRedraw(void);
	public:
		static void start(Window *window);
		static void end(Window *window);
	public:
		static void init(int argv, char **argc);
		static void mainLoop(bool inThread = true);
		static void stopMainLoop(void);
	};

	class Window:public DrawableContainer
	{
		friend class WindowManager;
		int window;
	private:
		//bool redisplayFlag;
		unsigned int lastRedraw;
		unsigned int currentRedraw;
		void checkRedraw(void);
		std::string name;
		//int window;
		Drawable *drawable;
	protected:
		int width;
		int height;
	public:
		Window(std::string name,Drawable *drawable = 0);
		virtual ~Window(void);
		virtual void draw(void);
		virtual void reshape(int w, int h);
		
		void setDrawable(Drawable *drawable);

		void redisplay(void);
		void getLocation(int &x,int &y);
		void setLocation(int x,int y);
		void getSize(int &w,int &h);
		void setSize(int w,int h);
	};
}
