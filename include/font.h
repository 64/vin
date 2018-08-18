#ifndef FONT_H
#define FONT_H

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

struct Glyph
{
    GLuint texture;
    unsigned int width, height;
    int bearingx, bearingy;
    long advance;
};

class Font
{
public:
    Font(FT_Library& library, const std::string& path, unsigned int height);
    Glyph get_glyph(char c) const;

private:
    std::unordered_map<char, Glyph> glyphs;    
};

#endif
