#include "Text.h"

Text::Text(std::string fontName, glm::vec3 pos, glm::vec2 anchor, float fontSize, glm::vec4 col, float spacing)
{
	font = fontName;
	this->pos = pos;
	this->center = anchor;
	this->size = fontSize;
	this->color = col;
	this->spacing = spacing;
}

Text* Text::SetText(std::string text)
{
	std::lock_guard<std::mutex> textLock(textMutex);
	for (Sprite* sp : textSprites)
		delete sp;

	textSprites.clear();

	glm::vec2 textSize = Font::getTextSize(font, text, spacing) * size;

	glm::vec3 startPos = (pos + glm::vec3((Font::getLetterWidth(font, text[0]) + spacing / 100.0f) * size, 0.0f, 0.0f)) - glm::vec3(textSize, 0.0f) + glm::vec3(center * -1.0f * textSize, 0.0f);
	textSprites = Font::ConvertText(font, text, startPos, spacing, size, color);

	this->text = text;
	return this;
}

Text* Text::ChangeSize(float size)
{
	if (textSprites.size() == 0)
		return this;
	this->size = size;
	return SetText(text);
}

Text* Text::SetPos(glm::vec3 pos)
{
	if (textSprites.size() == 0)
		return this;
	this->pos = pos;
	return SetText(text);
}

Text* Text::SetAnchor(glm::vec2 anchor)
{
	if (textSprites.size() == 0)
		return this;
	this->center = anchor;
	return SetText(text);
}

Text* Text::SetColor(glm::vec4 color)
{
	if (textSprites.size() == 0)
		return this;
	this->color = color;
	return SetText(text);
}

Text::~Text(){
	std::unique_lock<std::mutex> textLock(textMutex, std::defer_lock);
	textLock.lock();
	for(Sprite* sprite : textSprites){
		delete sprite;
	}

	textSprites.clear();

	textLock.unlock();
}