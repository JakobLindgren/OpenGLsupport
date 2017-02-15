#include "../Include/Font.h"
#include <GL/freeglut.h>
using namespace OpenGLsupport;


Font* Font::createFont(FontType font)
{
	switch (font)
	{
	case Bitmap_8_BY_13:        return new BitmapFont(GLUT_BITMAP_8_BY_13);
	case Bitmap_9_BY_15:        return new BitmapFont(GLUT_BITMAP_9_BY_15);
	case Bitmap_Times_Roman_10: return new BitmapFont(GLUT_BITMAP_TIMES_ROMAN_10);
	case Bitmap_Times_Roman_24: return new BitmapFont(GLUT_BITMAP_TIMES_ROMAN_24);
	case Bitmap_Helvetica_10:   return new BitmapFont(GLUT_BITMAP_HELVETICA_10);
	case Bitmap_Helvetica_12:   return new BitmapFont(GLUT_BITMAP_HELVETICA_12);
	case Bitmap_Helvetica_18:   return new BitmapFont(GLUT_BITMAP_HELVETICA_18);
	case Stroke_Roman:          return new StrokeFont(GLUT_STROKE_ROMAN);
	case Stroke_Mono_Roman:     return new StrokeFont(GLUT_STROKE_MONO_ROMAN);
	default:
		return 0;
	}
}




Font::Font(void *font) :font(font)
{
	scale = 1;
}
Font::~Font(){}

void Font::string(const std::string text) { string((const unsigned char*)text.c_str()); }
float Font::length(const std::string text, bool unscaled) { return length((const unsigned char*)text.c_str(), unscaled); }

void Font::setHeight(float characterHeight)
{
	scale = characterHeight / height(true);
}
float Font::getScale(void)
{
	return scale;
}



StrokeFont::StrokeFont(void *font) :Font(font) {}
void StrokeFont::character(int character)
{
	glutStrokeCharacter(font, character);
}
void StrokeFont::string(const unsigned char* text)
{
	glutStrokeString(font, text);
}
float StrokeFont::width(int character, bool unscaled)
{
	return glutStrokeWidth(font, character) * (unscaled ? 1.0f : scale);
}
float StrokeFont::length(const unsigned char* text, bool unscaled)
{
	return glutStrokeLength(font, text) * (unscaled ? 1.0f : scale);
}
float StrokeFont::height(bool unscaled)
{
	return glutStrokeHeight(font) * (unscaled ? 1.0f : scale);
}





BitmapFont::BitmapFont(void *font) :Font(font) {}
void BitmapFont::character(int character)
{
	glutBitmapCharacter(font, character);
}
void BitmapFont::string(const unsigned char* text)
{
	glutBitmapString(font, text);
}
float BitmapFont::width(int character, bool unscaled)
{
	return glutBitmapWidth(font, character) * (unscaled ? 1.0f : scale);
}
float BitmapFont::length(const unsigned char* text, bool unscaled)
{
	return glutBitmapLength(font, text) * (unscaled ? 1.0f : scale);
}
float BitmapFont::height(bool unscaled)
{
	return glutBitmapHeight(font) * (unscaled ? 1.0f : scale);
}
