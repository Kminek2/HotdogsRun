#pragma once
#include "Sprite.h"
class Font
{
private:
	static const std::string DEFAULT_FONT;

	struct CharData {
		float offset;
		float onTexSize;
		int pixSize;
	};

	std::vector<CharData> chars;
	unsigned spaceSize;
	unsigned fontHeight;

	static std::map<std::string, Font*> loadedFonts;

	static void LoadFontFromFile(std::string name, std::string filePath, unsigned fontHeight, unsigned spaceSize = 3);
public:
	static glm::vec2 getTextSize(std::string fontName ,std::string text, float spacing);
	static std::vector<Sprite*> ConvertText(std::string fontName, std::string text, glm::vec3 startPos, float spacing, float size, glm::vec4 col);
	static float getLetterWidth(std::string fontName, char letter);

	friend class Application;
};

