#include "DiagramWindow.h"
#include <GL/freeglut.h>
#include <sstream>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace OpenGLsupport;


DiagramArea::Axis::Axis(Font *font) :font(font)
{
	font->setHeight(float(FontHeight));
	firstSample = true;
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

	static const int maxValue = 4;
	//labels.clear();
	labels.resize(maxValue+1);

	for (int i = 0; i <= maxValue; i++)
	{
		Label &label = labels[i];
		label.value = (min*(i)+max*(maxValue - i)) / maxValue;
		//labels.push_back(label);
	}

	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		std::stringstream ss;

		ss.setf(std::ios::fixed, std::ios::floatfield);
		ss.precision(2);

		ss << it->value;
		it->str = ss.str();
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

	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(0, height);
	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		float h = (it->value - min) * scale;
		glVertex2f(0, h);
		glVertex2f(-LineLength, h);
	}
	glEnd();
	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		float h = (it->value - min) * scale;

		glPushMatrix();

		glTranslatef(-it->length - LineLength, h, 0);
		{
			float temp = font->getScale();
			glScalef(temp, temp, 1);
		}
		font->string(it->str);
		glPopMatrix();
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

	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(width, 0);
	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		float w = (it->value - min) * scale;

		glVertex2f(w, 0);
		glVertex2f(w, -LineLength);
	}
	glEnd();

	for (std::vector<Label>::iterator it = labels.begin(); it != labels.end(); ++it)
	{
		float w = (it->value - min) * scale;
		glPushMatrix();
		glTranslated(w - it->length *0.5, -LineLength - FontHeight, 0);
		{
			float temp = font->getScale();
			glScalef(temp, temp, 1);
		}
		font->string(it->str);
		glPopMatrix();
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
	glBegin(GL_LINES);
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
	glEnd();
}

void drawCross(double d)
{
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2d(-d, 0);
	glVertex2d(+d, 0);
	glVertex2d(0, -d);
	glVertex2d(0, +d);

	glVertex2d(+d, +d);
	glVertex2d(-d, -d);
	glVertex2d(+d, -d);
	glVertex2d(-d, +d);
	glEnd();
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

	glPushMatrix();
	{
		gluOrtho2D(0, width, 0, height);

		glPushMatrix();
		{
			glTranslatef(float(sizeLeft), float(sizeBottom), 0);

			glColor3d(0, 0, 0);
			vAxis.draw(diagramHeight);
			hAxis.draw(diagramWidth);

			glScalef(diagramWidth*0.5f, diagramHeight*0.5f, 1);
			glTranslatef(1, 1, 0);
			drawDiagram();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef((width - titleLength)*0.5f, float(height - FontHeight), 0);
			{
				float temp = font->getScale();
				glScalef(temp, temp, 1);
			}
			glColor3f(0,0,0);
			font->string(title);
		}
		glPopMatrix();
	}
	glPopMatrix();

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

		glBegin(GL_QUADS);
		setColor(index);
		glVertex2f(x0, 0);
		glVertex2f(x1, 0);
		glVertex2f(x1, float(count));
		glVertex2f(x0, float(count));
		glEnd();
		glBegin(GL_LINE_LOOP);
		glColor3ub(0, 0, 0);
		glVertex2f(x0, 0);
		glVertex2f(x1, 0);
		glVertex2f(x1, float(count));
		glVertex2f(x0, float(count));
		glEnd();


		x0 += 1.0f;
		x1 += 1.0f;

	}

	glEnd();


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


	glPushMatrix();
	gluOrtho2D(0, settings.bucketCount, 0, maxCount);
	int index = 0;
	for (std::map<int,HistogramChannel*>::iterator it = histogramChannels.begin(); it != histogramChannels.end(); ++it)
	{
		glBegin(GL_QUADS);
		it->second->draw(width, index++);
		glEnd();
		glTranslatef(width, 0, 0);
	}
	glPopMatrix();
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

	limits.minT = 0;
	limits.maxT = 1e-9;
	limits.minV = -1e-9;
	limits.maxV = +1e-9;

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
	add(&data, 1,channel);
}
void TimeSeries::add(float *dataVector, int count, int step,int channel)
{
	DataSeries* &series = data[channel];

	if (series == 0)
	{
		series = new DataSeries(settings.length);
		DataPoint temp = { 0,*dataVector };
		series->data.push_back(temp);
		dataVector += step;
		count--;
	}

	while (count--)
	{
		DataStorage &channelVector = series->data;
		
		DataPoint temp = channelVector.back();
		temp.t += settings.dt;
		temp.y = *dataVector;
		
		channelVector.push_back(temp);
		dataVector += step;
	}

	updateMinMax(channel);
}
void TimeSeries::drawDiagram(void)
{
	glPushMatrix();
	gluOrtho2D(limits.minT, limits.maxT, limits.minV, limits.maxV);

	for (std::map<int,DataSeries*>::iterator itr = data.begin(); itr != data.end(); itr++)
	{
		DataStorage &dataVector = itr->second->data;
		glBegin(GL_LINE_STRIP);
		setColor(itr->first);
		for (DataStorage::iterator dataPoint = dataVector.begin(); dataPoint != dataVector.end(); ++dataPoint)
		{
			glVertex2fv((*dataPoint).data);
		}
		glEnd();
	}

	glPopMatrix();
}

LineDiagram::LineDiagram(void):DiagramArea("Line Diagram")
{
	firstSample = true;
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

void LineDiagram::drawDiagram(void)
{
	int channel = 0;

	glPushMatrix();
	gluOrtho2D(minX, maxX, minY, maxY);

	for (std::map<int, std::vector<ChannelSample> >::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		ChannelSample *samples = it->second.data();
		int length = it->second.size();

		glBegin(GL_LINE_STRIP);
		setColor(channel++);
		while (length--)
		{
			glVertex2fv((samples++)->v);
		}
		glEnd();
	}

	glPopMatrix();
}

/*
Spectrogram::Spectrogram(int N,int H) :DiagramWindow("Spectrogram"),N(N),H(H)
{
	inBuffer = new fftw_complex[N];
	outBuffer = new fftw_complex[N];
	plan = fftw_plan_dft_1d(N,inBuffer,outBuffer,FFTW_FORWARD,FFTW_ESTIMATE);



	bufferData = new float[N*H];
	float *temp = bufferData;
	data = new float*[H];

	for (int i=0;i<H;i++)
	{
		data[i] = temp;
		temp += N;
	}
	currentIndex=0;

}
Spectrogram::~Spectrogram(void)
{
	fftw_destroy_plan(plan);

	delete data;
	delete bufferData;
}


void Spectrogram::add(float data)
{
	fftw_complex t = {data,0};
	inputQueue.push_back(t);
}
void Spectrogram::add(float *dataVector, int count, int step)
{
	while (count--)
	{
		add(*dataVector);
		dataVector += step;
	}
}

void Spectrogram::execute(void)
{
	while(inputQueue.size()>=N)
	{
		//memcpy(inBuffer,inputQueue.data(),N*sizeof(fftw_complex));
	}
}


void Spectrogram::drawDiagram(void)
{
	glPushMatrix();
	gluOrtho2D(0, N, 0, H);

	int bottomIndex = currentIndex-2;
	int topIndex = currentIndex-1;

	glBegin(GL_QUADS);
	for (int y=0;y<H;y++)
	{
		if (bottomIndex<0) bottomIndex+=H;
		if (topIndex<0) topIndex+=H;

		float *bottom = data[bottomIndex];
		float *top = data[topIndex];

		for (int x=1;x<N;x++)
		{
			float temp;

			temp = top[x-1];
			glColor3f(temp,temp,temp);
			glVertex2i(x-1,y+1);

			temp = top[x];
			glColor3f(temp,temp,temp);
			glVertex2i(x,y+1);

			temp = bottom[x];
			glColor3f(temp,temp,temp);
			glVertex2i(x,y);

			temp = bottom[x-1];
			glColor3f(temp,temp,temp);
			glVertex2i(x-1,y);
		}
		bottomIndex--;
		topIndex--;
	}
	glEnd();

	glPopMatrix();
}
*/