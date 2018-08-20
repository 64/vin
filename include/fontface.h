#ifndef FONT_H
#define FONT_H

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <string>

#include "util.h"

#define GUTTER_WIDTH (font.font_width() * 5)

struct Glyph
{
    GLuint texture;
    unsigned int width;
    unsigned int height;
    int bearingx;
    int bearingy;
    long advancex;
    long advancey;
};

class FontFace
{
public:
    FontFace(FT_Library& library, const std::string& path, unsigned int height, int _tabs_num_spaces);
    Glyph get_glyph(unsigned char c) const;
    int font_height() const;
    int font_width() const;
    int font_cleft() const;
    int num_spaces() const;

    static std::string get_system_font(const std::string& font_name);

private:
    std::unordered_map<char, Glyph> glyphs;
    int height;
    int width;
    int cleft;
    int tabs_num_spaces;
};

#endif
