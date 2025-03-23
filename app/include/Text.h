#pragma once
#include "Font.h"
class Text
{
private:
	std::string font;
	std::string text;

	std::vector<Sprite*> textSprites;
	glm::vec2 center;

	glm::vec3 pos;
	float size;
	float spacing;
	glm::vec4 color;
public:
	std::mutex textMutex;
	Text(std::string fontName, glm::vec3 pos, glm::vec2 anchor, float fontSize = 1, glm::vec4 col = glm::vec4(1), float spacing = 1);
	~Text();
	std::string getText() { return text; }
	void SetText(std::string text);
	float getSize() { return size; }
	void ChangeSize(float size);
	glm::vec3 GetPos() { return pos; }
	void SetPos(glm::vec3 pos);
	glm::vec2 GetAnchor() { return center; }
	void SetAnchor(glm::vec2 anchor);
	glm::vec4 GetColor() { return color; }
	void SetColor(glm::vec4 color);
};

