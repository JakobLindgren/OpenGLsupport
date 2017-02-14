#include "Diagram.h"
#include "Wrapper.h"
#include <GL/freeglut.h>
#include <sstream>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace OpenGLsupport;
static inline float interp1(float x1, float x2, float y1, float y2, float x)
{
	return (y2 - y1) * (x - x1) / (x2 - x1) + y1;
}
static inline double interp1(double x1, double x2, double y1, double y2, double x)
{
	return (y2 - y1) * (x - x1) / (x2 - x1) + y1;
}


DiagramArea::Axis::Axis(Font *font) :font(font),min(0),max(1)
{
	font->setHeight(float(FontHeight));
	firstSample = true;
	formatString = "%.2f";
	marks = 5;
}
void DiagramArea::Axis::setFormatString(std::string str)
{
	formatString = str;
}
void DiagramArea::Axis::setNumberOfMarks(unsigned int nr)
{
	marks=nr;
}

bool DiagramArea::Axis::setup(float min, float max)
{

	bool updated = false;

	if (firstSample)
	{
		this->min = min;
		this->max = max;
		updated = true;
	}
	else
	{

		if (this->min > min)
		{
			this->min = min;
			updated = true;
		}
		if (this->max < max)
		{
			this->max = max;
			updated = true;
		}
	}

	if (!updated) return false;

	//static const int maxValue = 4;
	//labels.clear();

	if (labels.size() != marks)
	{
		labels.resize(marks);
	}

	for (unsigned int i = 0; i < marks; i++)
	{
		Label &label = labels[i];
		label.value = interp1(0,marks-1,min,max,i);
	}

	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		char temp[256];
		sprintf(temp,formatString.c_str(),it->value);
		it->str = temp;
		it->length = font->length(it->str);
	}
	return true;
}

DiagramArea::VerticalAxis::VerticalAxis(OpenGLsupport::Font *font) :Axis(font)
{
	setup(0, 1);
}
bool DiagramArea::VerticalAxis::setup(float min, float max)
{
	if (Axis::setup(min, max) == false) return false;

	maxLength = 0;
	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		int temp = int(ceil(it->length));
		if (maxLength < temp) maxLength = temp;
	}
	return true;
}
int DiagramArea::VerticalAxis::topOffset(void)
{
	return FontHeight + EdgeOffset;
}
int DiagramArea::VerticalAxis::width(void)
{
	return maxLength + LineLength + EdgeOffset;
}
int DiagramArea::VerticalAxis::bottomOffset(void)
{
	return 0 + EdgeOffset;
}

void DiagramArea::VerticalAxis::draw(int height)
{
	float scale = height / (max - min);

	{
		GlBegin gb(DrawMode::lines);

		glVertex2i(0, 0);
		glVertex2i(0, height);
		for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
		{
			float h = (it->value - min) * scale;
			glVertex2f(0, h);
			glVertex2f(-LineLength, h);
		}
	}

	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		float h = (it->value - min) * scale;

		GlPushMatrix pm;
		glTranslatef(-it->length - LineLength, h, 0);
		{
			float temp = font->getScale();
			glScalef(temp, temp, 1);
		}
		font->string(it->str);

	}
}


DiagramArea::HorizontalAxis::HorizontalAxis(OpenGLsupport::Font *font) :Axis(font)
{
	setup(0, 1);
}
bool DiagramArea::HorizontalAxis::setup(float min, float max)
{
	if (Axis::setup(min, max) == false) return false;


	float leftValue = labels.front().value;
	float rightValue = labels.front().value;
	left = right = int(ceil(labels.front().length * 0.5f));

	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		float value = it->value;
		int temp = int(ceil(it->length * 0.5f));

		if (leftValue > value)
		{
			leftValue = value;
			left = temp;
		}
		if (rightValue < value)
		{
			rightValue = value;
			right = temp;
		}
	}
	return true;
}


int DiagramArea::HorizontalAxis::leftOffset(void)
{
	return left + EdgeOffset;
}
int DiagramArea::HorizontalAxis::height(void)
{
	return LineLength + FontHeight + EdgeOffset;
}
int DiagramArea::HorizontalAxis::rightOffset(void)
{
	return right + EdgeOffset;
}
void DiagramArea::HorizontalAxis::draw(int width)
{
	float scale = width / (max - min);

	{
		GlBegin gb(DrawMode::lines);
		glVertex2i(0, 0);
		glVertex2i(width, 0);
		for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
		{
			float w = (it->value - min) * scale;

			glVertex2f(w, 0);
			glVertex2f(w, -LineLength);
		}
	}
	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		float w = (it->value - min) * scale;
		GlPushMatrix pm;
		glTranslated(w - it->length *0.5, -LineLength - FontHeight, 0);
		{
			float temp = font->getScale();
			glScalef(temp, temp, 1);
		}
		font->string(it->str);

	}
}



void DiagramArea::setColor(int index)
{
	//http://tools.medialab.sciences-po.fr/iwanthue/
	static unsigned char colors[16][3] =
	{
		{ 92, 143,  69 },
		{ 201,  87, 203 },
		{ 208,  74,  50 },
		{ 135, 177, 204 },
		{ 91,  59, 108 },
		{ 205,  72, 123 },
		{ 205, 202,  68 },
		{ 116, 212, 177 },
		{ 112,  50,  50 },
		{ 116, 216,  83 },
		{ 67,  85,  86 },
		{ 205, 149, 166 },
		{ 121, 116, 206 },
		{ 198, 134,  63 },
		{ 200, 199, 149 },
		{ 86,  82,  39 }
	};
	glColor3ubv(colors[index & 0xf]);
}



DiagramArea::DiagramArea(std::string name) :
	font(OpenGLsupport::Font::createFont(OpenGLsupport::Font::Stroke_Roman)),
	vAxis(font),
	hAxis(font)
{
	setTitle(name);

}
DiagramArea::~DiagramArea(void)
{
	delete font;
}
void DiagramArea::setTitle(std::string value)
{
	title = value;
	titleLength = font->length(value);
}
void DiagramArea::drawDiagram(void)
{
	GlBegin gb(DrawMode::lines);
	glColor3ub(128, 128, 128);
	for (int a = 0; a < 4; a++)
	{
		float a_x = (a & 1 ? 1 : -1) * 1.0f;
		float a_y = (a & 2 ? 1 : -1) * 1.0f;
		for (int b = a + 1; b < 4; b++)
		{
			float b_x = (b & 1 ? 1 : -1) * 1.0f;
			float b_y = (b & 2 ? 1 : -1) * 1.0f;
			glVertex2f(a_x, a_y);
			glVertex2f(b_x, b_y);
		}
	}

}

void drawCross(double d)
{
	GlBegin gb(DrawMode::lines);

	glColor3f(0, 0, 0);
	glVertex2d(-d, 0);
	glVertex2d(+d, 0);
	glVertex2d(0, -d);
	glVertex2d(0, +d);

	glVertex2d(+d, +d);
	glVertex2d(-d, -d);
	glVertex2d(+d, -d);
	glVertex2d(-d, +d);
}


void DiagramArea::draw(void)
{
	int sizeRight = hAxis.rightOffset();
	int sizeTop = vAxis.topOffset();
	int sizeLeft = vAxis.width();
	int sizeBottom = hAxis.height();
	{
		int temp = hAxis.leftOffset();
		if (sizeLeft < temp)
		{
			sizeLeft = temp;
		}
	}
	{
		int temp = vAxis.bottomOffset();;
		if (sizeBottom < temp)
		{
			sizeBottom = temp;
		}
	}

	int diagramWidth = width - sizeLeft - sizeRight;
	int diagramHeight = height - sizeTop - sizeBottom - FontHeight;

	{
		GlPushMatrix pm;
		gluOrtho2D(0, width, 0, height);
		{

			GlPushMatrix pm;
			glTranslatef(float(sizeLeft), float(sizeBottom), 0);

			glColor3d(0, 0, 0);
			vAxis.draw(diagramHeight);
			hAxis.draw(diagramWidth);

			glScalef(diagramWidth*0.5f, diagramHeight*0.5f, 1);
			glTranslatef(1, 1, 0);
			drawDiagram();
		}

		{
			GlPushMatrix pm;
			glTranslatef((width - titleLength)*0.5f, float(height - FontHeight), 0);
			{
				float temp = font->getScale();
				glScalef(temp, temp, 1);
			}
			glColor3f(0,0,0);
			font->string(title);
		}

	}


}

Histogram::HistogramChannel::HistogramChannel(Settings &settings) :settings(settings)
{
}

void Histogram::HistogramChannel::clearBuckets(void)
{
	samplesCategorized = 0;
	buckets.clear();
	buckets.resize(settings.bucketCount);
}


void Histogram::HistogramChannel::process(void)
{
	while (samplesCategorized < size())
	{
		float value = (*this)[samplesCategorized++];
		for (unsigned int i = 0; i < settings.bucketCount; i++)
		{
			//if ((buckets[i].min <= value) && (value <= buckets[i].max))

			if ((settings.bucketMin[i] <= value) && (value <= settings.bucketMax[i]))
			{
				buckets[i]++;
				break;
			}
		}
	}
}

unsigned int Histogram::HistogramChannel::getMaxCount(void)
{
	unsigned int maxCount = 0;
	for (unsigned int i = 0; i < settings.bucketCount; i++)
	{
		unsigned int count = buckets[i];
		if (maxCount < count) maxCount = count;
	}
	return maxCount;
}

void Histogram::HistogramChannel::draw(float width, int index)
{
	float x0 = 0.0f;
	float x1 = width;

	for (unsigned int i = 0; i < settings.bucketCount; i++)
	{
		unsigned int count = buckets[i];

		{
			GlBegin gb(DrawMode::quads);
			setColor(index);
			glVertex2f(x0, 0);
			glVertex2f(x1, 0);
			glVertex2f(x1, float(count));
			glVertex2f(x0, float(count));
		}
		{
			GlBegin gb(DrawMode::line_loop);

			glColor3ub(0, 0, 0);
			glVertex2f(x0, 0);
			glVertex2f(x1, 0);
			glVertex2f(x1, float(count));
			glVertex2f(x0, float(count));
		}


		x0 += 1.0f;
		x1 += 1.0f;

	}
}



Histogram::Histogram(int bucketCount, int channels) :DiagramArea("Histogram")
{
	for (int i = 0; i < channels; i++)
	{
		histogramChannels[i] = new HistogramChannel(settings);
	}

	setBucketCount(bucketCount);

	lockMinMax = false;
	updatedMinMax = false;
	firstSample = true;
	maxCount = 0;
}

void Histogram::setBucketCount(int bucketCount)
{
	settings.bucketCount = bucketCount;

	settings.bucketMin.resize(bucketCount);
	settings.bucketMax.resize(bucketCount);

	for (std::map<int,HistogramChannel*>::iterator it = histogramChannels.begin(); it != histogramChannels.end(); ++it)
	{
		it->second->clearBuckets();
	}
	hAxis.setup(0.0f, float(bucketCount));
}
void Histogram::add(float data,int channel)
{
	add(&data, 1,channel);
}
void Histogram::add(float *data, int count,int channel)
{
	HistogramChannel *&histogramChannel = histogramChannels[channel];
	if (histogramChannel == 0)
	{
		histogramChannel = new HistogramChannel(settings);
		histogramChannel->clearBuckets();
	}

	while (count--)
	{
		float value = *(data++);
		if (firstSample)
		{
			minSample = value;
			maxSample = value;
			firstSample = false;
			updatedMinMax = true;
		}
		else
		{
			if (minSample > value)
			{
				minSample = value;
				updatedMinMax = true;
			}
			if (maxSample < value)
			{
				maxSample = value;
				updatedMinMax = true;
			}
		}
		histogramChannel->push_back(value);
	}
}
void Histogram::clearBuckets(void)
{
	for (std::map<int,HistogramChannel*>::iterator it = histogramChannels.begin(); it != histogramChannels.end(); ++it)
	{
		it->second->clearBuckets();
	}
	vAxis.setup(0, 1);
	maxCount = 0;
}

void Histogram::calculateBucketMinMax(void)
{
	float dist = (maxSample - minSample) / settings.bucketCount;
	float max = minSample;

	for (unsigned int i = 0; i < settings.bucketCount; i++)
	{
		settings.bucketMin[i] = max;
		max += dist;
		settings.bucketMax[i] = max;
	}
	updatedMinMax = false;
	clearBuckets();
}


void Histogram::process(void)
{
	if (updatedMinMax)
		calculateBucketMinMax();

	unsigned int newMaxCount = 0;

	for (std::map<int,HistogramChannel*>::iterator it = histogramChannels.begin(); it != histogramChannels.end(); ++it)
	{
		it->second->process();
		unsigned int temp = it->second->getMaxCount();

		if (newMaxCount < temp) newMaxCount = temp;
	}

	if (maxCount < newMaxCount)
	{
		maxCount = newMaxCount;
		vAxis.setup(0.0f, float(maxCount));

	}

}

void Histogram::drawDiagram(void)
{
	if (histogramChannels.size() == 0)
	{
		DiagramArea::draw();
		return;
	}
	float width = 1.0f / histogramChannels.size();


	{
		GlPushMatrix pm;
		gluOrtho2D(0, settings.bucketCount, 0, maxCount);
		int index = 0;
		for (std::map<int, HistogramChannel*>::iterator it = histogramChannels.begin(); it != histogramChannels.end(); ++it)
		{
			it->second->draw(width, index++);
			glTranslatef(width, 0, 0);
		}
	}
}

void Histogram::setMinMax(float min, float max)
{
	lockMinMax = true;
	minSample = min;
	maxSample = max;

	calculateBucketMinMax();
}


TimeSeries::TimeSeries(std::string name,float T, float dt):DiagramArea(name)
{
	settings.length = (unsigned int)(T / dt);
	settings.dt = dt;
	settings.keepLimits = false;

	limits.minT = 0.0f;
	limits.maxT = 1e-9f;
	limits.minV = -1e-9f;
	limits.maxV = +1e-9f;

	vAxis.setup(limits.minV, limits.maxV);
	hAxis.setup(limits.minT, limits.maxT);


	//firstSample = true;
}

void TimeSeries::setKeepLimits(bool value)
{
	settings.keepLimits = value;
}

void TimeSeries::updateMinMax(int channel)
{
	Limits l = limits;
	{
		DataStorage &dataVector = data[channel]->data;
		float &minV = data[channel]->minV;
		float &maxV = data[channel]->maxV;

		minV = maxV = dataVector.front().y;
		
		for (DataStorage::iterator itr = dataVector.begin(); itr != dataVector.end(); ++itr)
		{
			float temp = (*itr).y;
			if (minV > temp) minV = temp;
			if (maxV < temp) maxV = temp;
		}


		l.minT = dataVector.front().t;
		l.maxT = dataVector.back().t;

		if ((settings.keepLimits==false) || (l.minV > minV)) l.minV = minV;
		if ((settings.keepLimits==false) || (l.maxV < maxV)) l.maxV = maxV;
	}
	
	for (std::map<int,DataSeries*>::iterator itr = data.begin(); itr != data.end(); ++itr)
	{
		DataSeries& series = *(itr->second);

		float minT = series.data.front().t;
		float maxT = series.data.back().t;
		float minV = series.minV;
		float maxV = series.maxV;

		if (l.minT > minT) l.minT = minT;
		if (l.maxT < maxT) l.maxT = maxT;
		if (l.minV > minV) l.minV = minV;
		if (l.maxV < maxV) l.maxV = maxV;
	}

	limits = l;
	vAxis.setup(limits.minV, limits.maxV);
	hAxis.setup(limits.minT, limits.maxT);
}
void TimeSeries::add(float data,int channel)
{
	add(&data, 1,1,channel);
}
void TimeSeries::add(float *dataVector, int count, int step,int channel)
{
	DataSeries* &series = data[channel];

	if (series == 0)
	{
		series = new DataSeries(settings.length);
		DataPoint temp = {0,{ 0,*dataVector }};
		series->data.push_back(temp);
		series->minV = *dataVector;
		series->maxV = *dataVector;
		dataVector += step;
		count--;
	}

	while (count--)
	{
		DataStorage &channelVector = series->data;
		
		DataPoint temp = channelVector.back();
		temp.index++;
		temp.t = settings.dt * temp.index;
		//temp.t+=settings.dt;
		temp.y = *dataVector;
		
		channelVector.push_back(temp);
		dataVector += step;
	}

	updateMinMax(channel);
}
void TimeSeries::drawDiagram(void)
{
	GlPushMatrix pm;
	gluOrtho2D(limits.minT, limits.maxT, limits.minV, limits.maxV);

	for (std::map<int, DataSeries*>::iterator itr = data.begin(); itr != data.end(); itr++)
	{
		DataStorage &dataVector = itr->second->data;
		GlBegin gb(DrawMode::line_strip);

		setColor(itr->first);
		for (DataStorage::iterator dataPoint = dataVector.begin(); dataPoint != dataVector.end(); ++dataPoint)
		{
			glVertex2fv((*dataPoint).data);
		}
	}
}

LineDiagram::LineDiagram(std::string name):DiagramArea(name)
{
	firstSample = true;
}
void LineDiagram::add(double *x, double *y, int count, int channel)
{
	std::vector<ChannelSample> &channelVector = channels[channel];
	channelVector.reserve(channelVector.size() + count);

	while (count--)
	{
		ChannelSample value = { *(x++),*(y++) };

		if (firstSample)
		{
			minX = value.x;
			maxX = value.x;
			minY = value.y;
			maxY = value.y;
			firstSample = false;
		}
		else
		{
			if (minX > value.x)
			{
				minX = value.x;
			}
			if (maxX < value.x)
			{
				maxX = value.x;
			}
			if (minY > value.y)
			{
				minY = value.y;
			}
			if (maxY < value.y)
			{
				maxY = value.y;
			}
		}
		channelVector.push_back(value);
	}
	hAxis.setup(minX, maxX);
	vAxis.setup(minY, maxY);
}
void LineDiagram::add(float *x, float *y, int count, int channel)
{
	std::vector<ChannelSample> &channelVector = channels[channel];
	channelVector.reserve(channelVector.size() + count);

	while (count--)
	{
		ChannelSample value = { *(x++),*(y++) };

		if (firstSample)
		{
			minX = value.x;
			maxX = value.x;
			minY = value.y;
			maxY = value.y;
			firstSample = false;
		}
		else
		{
			if (minX > value.x)
			{
				minX = value.x;
			}
			if (maxX < value.x)
			{
				maxX = value.x;
			}
			if (minY > value.y)
			{
				minY = value.y;
			}
			if (maxY < value.y)
			{
				maxY = value.y;
			}
		}
		channelVector.push_back(value);
	}
	hAxis.setup(minX, maxX);
	vAxis.setup(minY, maxY);
}

void LineDiagram::add(ChannelSample *sample, int count,int channel)
{
	std::vector<ChannelSample> &channelVector = channels[channel];
	channelVector.reserve(channelVector.size() + count);

	while (count--)
	{
		ChannelSample value = *(sample++);
		if (firstSample)
		{
			minX = value.x;
			maxX = value.x;
			minY = value.y;
			maxY = value.y;
			firstSample = false;
		}
		else
		{
			if (minX > value.x)
			{
				minX = value.x;
			}
			if (maxX < value.x)
			{
				maxX = value.x;
			}
			if (minY > value.y)
			{
				minY = value.y;
			}
			if (maxY < value.y)
			{
				maxY = value.y;
			}
		}
		channelVector.push_back(value);
	}
	hAxis.setup(minX, maxX);
	vAxis.setup(minY, maxY);
}

void OpenGLsupport::LineDiagram::clear(int channel)
{
	std::vector<ChannelSample> &channelVector = channels[channel];
	channelVector.clear();
}

void LineDiagram::drawDiagram(void)
{
	int channel = 0;

	GlPushMatrix pm;
	gluOrtho2D(minX, maxX, minY, maxY);

	for (std::map<int, std::vector<ChannelSample> >::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		ChannelSample *samples = it->second.data();
		int length = it->second.size();

		{
			GlBegin gb(DrawMode::line_strip);

			setColor(channel++);
			while (length--)
			{
				glVertex2fv((samples++)->v);
			}
		}
	}
}


SpectrogramBase::SpectrogramBase(std::string name,int N,int H,float fs, bool useComplexBuffer) :DiagramArea(name),N(N),H(H),fs(fs), useComplexBuffer(useComplexBuffer)
{
	advancement = N / 2;
	colorPalette = 0;
	scale = 2.0/float(N);

	bufferData = new RGB[N*H];
	memset(bufferData,0,N*H*sizeof(RGB));

	RGB *temp = bufferData;


	data = new RGB*[H];
	for (int i=0;i<H;i++)
	{
		data[i] = temp;
		temp += N;
	}

	xValues = new float[N];
	bool logX = false;

	if (logX)
	{
		float minX = log(1.0f/N);
		for (int i=0;i<N;i++)
		{
			float temp = log(float(i+1)/float(N));
			xValues[i] = interp1(minX,0,0,1,temp);
		}
	}
	else
	{
		for (int i=0;i<N;i++)
		{
			xValues[i] = float(i)/float(N);
		}
	}
	doLog=true;


	hAxis.setFormatString("%.0f");
	hAxis.setNumberOfMarks(11);
	setFrequencyRange(0,fs);

	vAxis.setFormatString("%.1f");
	vAxis.setup(0,H*advancement / fs);

	//hAxis.setup(0,fs);





}
SpectrogramBase::~SpectrogramBase(void)
{
	delete data;
	delete bufferData;
	delete xValues;
}

void OpenGLsupport::SpectrogramBase::setAdvancement(int value)
{
	advancement = value;
}

void SpectrogramBase::setFrequencyRange(float minF,float maxF)
{
	{
		minIndexFreq = 0;
		while (xValues[minIndexFreq]*fs < minF )
		{
			minIndexFreq++;
		}
	}
	{
		maxIndexFreq = N;
		while (xValues[--maxIndexFreq]*fs > maxF );
	}

	hAxis.setup(xValues[minIndexFreq]*fs,xValues[maxIndexFreq]*fs);
}


void SpectrogramBase::addAsComplex(float data)
{
	inputQueue.push_back(data);
	inputQueue.push_back(0);
}
void SpectrogramBase::add(float *dataVector, int count, int step)
{
	inputQueue.reserve(inputQueue.size() + count);

	if (useComplexBuffer)
	{
		while (count--)
		{
			addAsComplex(*dataVector);
			dataVector += step;
		}
	}
	else
	{
		
		while (count--)
		{
			inputQueue.push_back(*dataVector);
			dataVector += step;
		}
	}
}
void SpectrogramBase::setColorPalette(ColorPalette *colorPalette)
{
	this->colorPalette = colorPalette;
}



void SpectrogramBase::rotateLinePointers(void)
{
	RGB *temp = data[0];
	for (unsigned int i=1;i<H;i++)
	{
		data[i-1] = data[i];
	}
	data[H-1] = temp;
}

void SpectrogramBase::setData(float *frequencyData)
{
	{
		RGB *writeLocation = data[0]+minIndexFreq;
		float *readLocation = frequencyData + minIndexFreq;
		for (unsigned int i=0;i<=N;i++)
		{
			float real = *(readLocation++);
			float imag = *(readLocation++);

			float temp =  sqrt(real*real+imag*imag) * scale;
			temp = 20*log10(temp);

			writeLocation->raw = interp1(-320,0,0,1,temp);
			if (writeLocation->raw<0)
				writeLocation->raw = 0;


			if (colorPalette)
			{
				colorPalette->getColor(temp,writeLocation->color);
			}
			else
			{
				writeLocation->R = writeLocation->raw;
				writeLocation->G = writeLocation->raw;
				writeLocation->B = writeLocation->raw;
			}



			writeLocation++;
		}
	}


	rotateLinePointers();
}

void SpectrogramBase::drawDiagram(void)
{
	glPushMatrix();
	gluOrtho2D(xValues[minIndexFreq], xValues[maxIndexFreq], H-2,0);

	for (unsigned int y=0;y<H-2;y++)
	{
		RGB *line1 = data[y+2] + minIndexFreq;
		RGB *line2 = data[y+1] + minIndexFreq;

		glBegin(GL_QUAD_STRIP);
		for (int i=minIndexFreq;i<=maxIndexFreq;i++)
		{
			//glColor3f(*line1,*line1,*line1); glVertex3f(xValues[i],y+1,*line1); line1++;
			//glColor3f(*line2,*line2,*line2); glVertex3f(xValues[i],y+0,*line2); line2++;

			glColor3fv(line1->color); glVertex3f(xValues[i],y+1,line1->raw); line1++;
			glColor3fv(line2->color); glVertex3f(xValues[i],y+0,line2->raw); line2++;
		}
		glEnd();
	}


	glPopMatrix();
}

#if defined(USE_FFTS)
Spectrogram::Spectrogram(std::string name,int N,int H,float fs):SpectrogramBase(name,N,H,fs)
{
	__ffts.outBuffer = new float[2*N];
	__ffts.plan = ffts_init_1d(N, 1);

}
Spectrogram::~Spectrogram(void)
{
	ffts_free(__ffts.plan);
	delete[] __ffts.outBuffer;
}
bool Spectrogram::execute(void)
{
	std::cout<<"Spectrogram::execute_ffts"<<std::endl;

	bool res = false;
	while(inputQueue.size()>=(2*N))
	{
		ffts_execute(__ffts.plan,inputQueue.data(), __ffts.outBuffer);
		inputQueue.erase(inputQueue.begin(),inputQueue.begin()+2*advancement);
		setData(__ffts.outBuffer);
		res = true;
	}
	return res;
}
#endif

#if defined(USE_JAKOB_FFT)
Spectrogram::Spectrogram(std::string name, int N, int H, float fs) :SpectrogramBase(name, N, H, fs,false),fft(N)
{
	outData = new float[N];
}
Spectrogram::~Spectrogram(void)
{
	delete[] outData;
}
bool Spectrogram::execute(void)
{
	std::cout << "Spectrogram::execute_fft" << std::endl;

	bool res = false;
	while (inputQueue.size() >= N)
	{
		fft.load(inputQueue.data(), N);
		fft.processFFT();
		fft.unload(outData);
		setData(outData);
		res = true;
	}
	return res;
}
#endif
