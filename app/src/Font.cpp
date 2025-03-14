#include "Font.h"
#include <iostream>
#include <fstream>

const std::string Font::DEFAULT_FONT = "BaseFont";
const unsigned startingFromChar = 32;
std::map<std::string, Font*> Font::loadedFonts;

void Font::LoadFontFromFile(std::string name, std::string filePath, unsigned fontHeight, unsigned spaceSize)
{
    Font* newFont = new Font();
    newFont->chars.resize(CHAR_MAX - startingFromChar);

	std::ifstream file;
	file.open(filePath);

    char temp;

    for (int i = 0; i < CHAR_MAX - startingFromChar; i++)
    {
        file.get(temp);
        while (temp != ' ')
        {
            file.get(temp);
        }
        file.get(temp);
        while (temp != ' ')
        {
            file.get(temp);
        }

        float right;
        file >> newFont->chars[i].offset;
        file >> right;
        newFont->chars[i].onTexSize = right - newFont->chars[i].offset;
        file >> newFont->chars[i].pixSize;
    }

    file.close();

    newFont->spaceSize = spaceSize;
    newFont->fontHeight = fontHeight;
    loadedFonts[name] = newFont;
}

glm::vec2 Font::getTextSize(std::string fontName, std::string text, float spacing)
{
    Font* font = loadedFonts[fontName];

    unsigned size = 0;

    for (char ch : text ) {
        if (ch == ' ')
            size += font->spaceSize;
        else
            size += font->chars[ch - startingFromChar].pixSize;
    }

    size -= font->chars[text[0] - startingFromChar].pixSize;
    size -= font->chars[text[text.size() - 1] - startingFromChar].pixSize;
    size += spacing * (text.size() - 1);

    return glm::vec2(size, font->fontHeight / 2.0f) / 100.0f;
}

std::vector<Sprite*> Font::ConvertText(std::string fontName, std::string text, glm::vec3 startPos, float spacing, float size, glm::vec4 col)
{
    Font* font = loadedFonts[fontName];
    std::vector<Sprite*> textSprites;

    size /= 100;
    glm::vec3 pos = startPos;
    for (int i = 0; i < text.size(); i++) {
        char ch = text[i];

        if (ch == ' ')
            pos += font->spaceSize * size * glm::vec3(1, 0, 0);
        else {
            Sprite* sp = new Sprite(fontName, 0, glm::vec2(font->chars[ch - startingFromChar].onTexSize, 1), glm::vec2(font->chars[ch - startingFromChar].offset, 0), col);
            sp->rectTransform->MoveTo(pos);
            sp->rectTransform->ScaleTo(glm::vec2(font->chars[ch - startingFromChar].pixSize, font->fontHeight) * size);
            if (i + 1 < text.size())
                pos += (font->chars[ch - startingFromChar].pixSize + font->chars[text[i + 1] - startingFromChar].pixSize + spacing) * size * glm::vec3(1, 0, 0);
        }
    }
    return textSprites;
}