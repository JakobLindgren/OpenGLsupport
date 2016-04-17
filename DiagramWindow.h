#pragma once

#include <map>
#include <vector>
#include "OpenGLsupport/Font.h"
#include "OpenGLsupport/Drawable.h"
#include "CircularBuffer.h"

//#include <fftw3.h>


namespace OpenGLsupport
{

	class DiagramArea :public Drawable
	{
	private:
		Font *font;
		std::string title;
		float titleLength;
		void setTitle(std::string value);
	protected:
		static void setColor(int index);
	protected:
		static const int FontHeight = 20;

		class Axis
		{
		protected:
			static const int LineLength = 20;
			static const int EdgeOffset = 3;

			Font *font;
			bool firstSample;
			float min;
			float max;

			struct Label
			{
				float value;
				std::string str;
				float length;
			};

			std::vector<Label> labels;


			Axis(Font *font);
			virtual bool setup(float min, float max);
		public:

		};

		class VerticalAxis :public Axis
		{
		private:
			int maxLength;
		public:

			VerticalAxis(Font *font);
			bool setup(float min, float max) override;

			int topOffset(void);
			int width(void);
			int bottomOffset(void);

			void draw(int height);
		};

		class HorizontalAxis :public Axis
		{
		private:
			int left;
			int right;
		public:

			HorizontalAxis(Font *font);
			bool setup(float min, float max) override;

			int leftOffset(void);
			int height(void);
			int rightOffset(void);

			void draw(int width);
		};

		VerticalAxis vAxis;
		HorizontalAxis hAxis;

	public:
		DiagramArea(std::string name);
		virtual ~DiagramArea(void);

		void draw(void) override;
		virtual void drawDiagram(void);
	};



	class Histogram : public DiagramArea
	{
	private:
		struct Settings
		{
			unsigned int bucketCount;
			std::vector<float> bucketMin;
			std::vector<float> bucketMax;
		};

		class HistogramChannel :public std::vector<float>
		{
			friend class Histogram;
		public:
		private:
			Settings &settings;

			unsigned int samplesProcessed;
			unsigned int samplesCategorized;

			std::vector<int> buckets;
		public:
			HistogramChannel(Settings &settings);
			void clearBuckets(void);
			void process(void);

			unsigned int getMaxCount(void);

			void draw(float width, int index);
		};

		std::map<int, HistogramChannel*> histogramChannels;
		Settings settings;

		bool firstSample;
		bool updatedMinMax;
		unsigned int maxCount;
		float minSample;
		float maxSample;
		bool lockMinMax;


		void calculateBucketMinMax(void);
	public:
		Histogram(int bucketCount, int channels = 0);
		void setBucketCount(int bucketCount);

		void add(float data, int channel = 0);
		void add(float *data, int count, int channel = 0);
		void clearBuckets(void);


		void process(void);

		void drawDiagram(void) override;

		void setMinMax(float min, float max);
	};




	class TimeSeries : public DiagramArea
	{
	private:

		union DataPoint
		{
			struct
			{
				float t;
				float y;
			};
			float data[2];
		};
		//typedef std::vector<DataPoint> DataStorage;
		typedef CircularBuffer<DataPoint> DataStorage;

		struct DataSeries
		{
			//CircularBuffer<DataPoint> data;
			DataStorage data;
			float minV, maxV;

			DataSeries(unsigned int length) :data(length) {}
		};
		struct Settings
		{
			float dt;
			unsigned int length;
			bool keepLimits;
		};
		struct Limits
		{
			float minT, maxT;
			float minV, maxV;
		};


		Settings settings;
		Limits limits;
		//bool firstSample;

		void updateMinMax(int channel);

		std::map<int, DataSeries*> data;
	public:
		TimeSeries(std::string name, float T, float dt = 1);
		void add(float data, int channel = 0);
		void add(float *dataVector, int count, int step = 1, int channel = 0);
		void drawDiagram(void) override;
		void setKeepLimits(bool value);
	};


	class LineDiagram : public DiagramArea
	{
	public:
		union ChannelSample
		{
			struct
			{
				float x;
				float y;
			};
			float v[2];
		};
	private:
		std::map<int, std::vector<ChannelSample> > channels;
		bool firstSample;
		float minX;
		float maxX;
		float minY;
		float maxY;
	public:
		LineDiagram(void);
		inline void add(float x, float y, int channel = 0)
		{
			ChannelSample sample = { x,y };
			add(sample, channel);
		}
		inline void add(float *x, float *y, int count, int channel = 0)
		{
			while (count--)
			{
				add(*(x++), *(y++), channel);
			}
		}
		inline void add(ChannelSample sample, int channel = 0)
		{
			add(&sample, 1, channel);
		}
		void add(ChannelSample *sample, int count, int channel = 0);

		void drawDiagram(void) override;
	};
	/*

	class Spectrogram : public DiagramWindow
	{
	private:
		fftw_complex *inBuffer;
		fftw_complex *outBuffer;
		fftw_plan plan;

		float **data;
		float *bufferData;

		int currentIndex;
		int N,H;

		std::vector<fftw_complex> inputQueue;
	public:
		Spectrogram(int N,int H);
		virtual ~Spectrogram(void);


		virtual void drawDiagram(void);
		void add(float data);
		void add(float *dataVector, int count, int step = 1);
		void execute(void);
	};
	*/
}