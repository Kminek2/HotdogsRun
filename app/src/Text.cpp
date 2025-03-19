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

void Text::SetText(std::string text)
{
	for (Sprite* sp : textSprites)
		delete sp;

	textSprites.clear();

	glm::vec2 textSize = Font::getTextSize(font, text, spacing) * size;

	glm::vec3 startPos = (pos + glm::vec3((Font::getLetterWidth(font, text[0]) + spacing / 100.0f) * size, 0.0f, 0.0f)) - glm::vec3(textSize, 0.0f) + glm::vec3(center * -1.0f * textSize, 0.0f);
	textSprites = Font::ConvertText(font, text, startPos, spacing, size, color);

	this->text = text;
}

void Text::ChangeSize(float size)
{
	this->size = size;
	SetText(text);
}

void Text::SetPos(glm::vec3 pos)
{
	this->pos = pos;
	SetText(text);
}

void Text::SetAnchor(glm::vec2 anchor)
{
	this->center = anchor;
	SetText(text);
}

void Text::SetColor(glm::vec4 color)
{
	this->color = color;
	SetText(text);
}

Text::~Text(){
	for(Sprite* sprite : textSprites){
		delete sprite;
	}
}