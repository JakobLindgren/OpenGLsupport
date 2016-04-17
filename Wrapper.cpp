#include "Wrapper.h"
using namespace OpenGLsupport;

GlBegin::GlBegin(Mode mode)
{
	glBegin(mode);
}
GlBegin::~GlBegin(void)
{
	glEnd();
}

GlPushMatrix::GlPushMatrix(void)
{
	glPushMatrix();
}
GlPushMatrix::~GlPushMatrix(void)
{
	glPopMatrix();
}
