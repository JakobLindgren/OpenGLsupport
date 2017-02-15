#pragma once

#include <map>
#include <vector>
#include "Font.h"
#include "Drawable.h"
#if defined(USE_JAKOB_FFT)
#include "FFT.h"
#endif

#include "ColorPalette.h"
//#include "CircularBuffer.h"

//#include <complex>
//#include <fftw3.h>
#if defined(USE_FFTS)
#include <ffts.h>
#endif



namespace OpenGLsupport
{
template<typename T>
	class CircularBuffer
	{
	private:
		static const unsigned int bits = 1 + 10 + 10;
		static const int allocationSize = 1 << bits;
		T *data;
		unsigned int startIndex;
		unsigned int endIndex;
		unsigned int length;
		unsigned int maxLength;

		static unsigned int stepIndex(unsigned int &index)
		{
			index++;
			if (index == allocationSize)
				index = 0;
			return index;
		}
	public:
		class iterator
		{
			friend class CircularBuffer;
		private:
			unsigned int index;
			T *data;
			iterator(unsigned int index, T *data) :index(index), data(data) {}
		public:
			bool operator!=(const iterator& other) const
			{
				return index != other.index;
			}
			T& operator* (void)
			{
				return data[index];
			}
			void operator++ (void)
			{
				if (++index == allocationSize)
					index = 0;
			}

		};
		CircularBuffer(unsigned int maxLength)
		{
			this->data = new T[allocationSize];
			this->startIndex = 0;
			this->endIndex = 0;
			this->length = 0;
			this->maxLength = maxLength;
		}
		~CircularBuffer(void)
		{
			delete data;
		}

		unsigned int size(void) const
		{
			return (endIndex - startIndex) & (allocationSize - 1);
		}

		void push_back(T v)
		{
			if (size() == maxLength)
				if (++startIndex == allocationSize)
					startIndex = 0;

			data[endIndex] = v;
			if (++endIndex == allocationSize)
				endIndex = 0;
		}

		iterator begin(void)
		{
			return iterator(startIndex, data);
		}
		iterator end(void)
		{
			return iterator(endIndex, data);
		}
		T& front(void)
		{
			return data[startIndex];
		}
		T& back(void)
		{
			int temp = endIndex;
			if (temp == 0) temp = allocationSize;
			return data[temp - 1];
		}
	};

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
			float minValue;
			float maxValue;
			unsigned int marks;

			struct Label
			{
				float value;
				std::string str;
				float length;
			};

			std::vector<Label> labels;
			std::string formatString;

			Axis(Font *font);
			virtual bool setup(float min, float max);
		public:
		void setFormatString(std::string str);
		void setNumberOfMarks(unsigned int nr);
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

		struct DataPoint
		{
			int index;
			union
			{
				struct
				{
					float t;
					float y;
				};
				float data[2];
			};
		};
		//typedef std::vector<DataPoint> DataStorage;
		typedef CircularBuffer<DataPoint> DataStorage;

		struct DataSeries
		{
			//CircularBuffer<DataPoint> data;
			DataStorage data;
			float minV, maxV;

			DataSeries(unsigned int length) :data(length),minV(0),maxV(0) {}
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
		LineDiagram(std::string name);
		inline void add(float x, float y, int channel = 0)
		{
			ChannelSample sample = { x,y };
			add(sample, channel);
		}
		inline void add(ChannelSample sample, int channel = 0)
		{
			add(&sample, 1, channel);
		}
		inline void add(std::vector<double> &x, std::vector<double> &y, int channel = 0)
		{
			int lenX = x.size();
			int lenY = y.size();
			int length = lenX < lenY ? lenX : lenY;
			add(x.data(), y.data(), length, channel);
		}
		void add(ChannelSample *sample, int count, int channel = 0);
		void add(double *x, double *y, int count, int channel = 0);
		void add(float *x, float *y, int count, int channel = 0);

		void clear(int channel = 0);

		void drawDiagram(void) override;
	};


class SpectrogramBase : public DiagramArea
{
private:
		/*
	struct
	{
		float *outBuffer;
		ffts_plan_t *plan;
	} __ffts;
*/
	union RGB
	{
		struct
		{
			float R,G,B,raw;
		};
		float color[4];
	};
	RGB **data;
	RGB *bufferData;
	float *xValues;
	float scale;


	int minIndexFreq,maxIndexFreq;

	unsigned int H;

	float fs;
	bool doLog;

	ColorPalette *colorPalette;

protected:
	unsigned int N;
	unsigned int advancement;

	std::vector<float> inputQueue;
	void setData(float *frequencyData);
	void rotateLinePointers(void);

	bool useComplexBuffer;
	void addAsComplex(float data);
public:
	SpectrogramBase(std::string name,int N,int H,float fs,bool useComplexBuffer);
	virtual ~SpectrogramBase(void);


	virtual void drawDiagram(void);
	virtual void add(float *dataVector, int count, int step = 1);
	void setColorPalette(ColorPalette *colorPalette);
	void setAdvancement(int value);
	virtual bool execute(void) = 0;

	void setFrequencyRange(float minF,float maxF);
};

#if defined(USE_FFTS)
class Spectrogram : public SpectrogramBase
{
	struct
	{
		float *outBuffer;
		ffts_plan_t *plan;
	} __ffts;
private:
public:
	Spectrogram(std::string name,int N,int H,float fs);
	virtual ~Spectrogram(void);

	bool execute(void) override;
};
#endif

#if defined(USE_JAKOB_FFT)
class Spectrogram : public SpectrogramBase
{
private:
	FFT fft;
	float *outData;
public:
	Spectrogram(std::string name, int N, int H, float fs);
	virtual ~Spectrogram(void);

	bool execute(void) override;
};
#endif
}
