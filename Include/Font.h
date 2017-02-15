
#pragma once

#include <string>

namespace OpenGLsupport
{
	class Font
	{
	public:
		enum FontType
		{
			Bitmap_8_BY_13,
			Bitmap_9_BY_15,
			Bitmap_Times_Roman_10,
			Bitmap_Times_Roman_24,
			Bitmap_Helvetica_10,
			Bitmap_Helvetica_12,
			Bitmap_Helvetica_18,
			Stroke_Roman,
			Stroke_Mono_Roman
		};
	private:

	protected:
		void *font;
		float scale;

		Font(void *font);
	public:
		virtual ~Font();
		virtual void character(int character) = 0;

		void string(const std::string text);
		virtual void string(const unsigned char* text) = 0;

		float length(const std::string text, bool unscaled = false);
		virtual float length(const unsigned char* text, bool unscaled = false) = 0;

		virtual float width(int character, bool unscaled = false) = 0;
		virtual float height(bool unscaled = false) = 0;

		static Font* createFont(FontType font);

		void setHeight(float characterHeight);
		float getScale(void);
	};
	class StrokeFont :public Font
	{
	public:
		StrokeFont(void *font);
		void character(int character) override;
		void string(const unsigned char* text) override;

		float width(int character, bool unscaled) override;
		float length(const unsigned char* text, bool unscaled) override;
		float height(bool unscaled) override;
	};
	class BitmapFont :public Font
	{
	public:
		BitmapFont(void *font);

		void character(int character) override;
		void string(const unsigned char* text) override;

		float width(int character, bool unscaled) override;
		float length(const unsigned char* text, bool unscaled) override;
		float height(bool unscaled) override;
	};
}
