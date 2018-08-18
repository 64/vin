#include <iostream>
#include <algorithm>

#include "font.h"

[[noreturn]] void error(const std::string& msg);

Font::Font(FT_Library& library, const std::string& path, unsigned int height)
{
    FT_Face face;
    if (FT_New_Face(library, path.c_str(), 0, &face))
        error("Failed to load font from path: " + path);

    FT_Set_Pixel_Sizes(face, 0, height);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte alignment

    // Loop through all ASCII characters and render them to textures
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            error("Failed to load font character '" + std::string{static_cast<char>(c)} + "' from path: " + path);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        max_height = std::max(face->glyph->bitmap.rows, max_height);

        glyphs.emplace(c, Glyph { texture, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap_left, face->glyph->bitmap_top, face->glyph->advance.x });
    }

    FT_Done_Face(face);
}

Glyph Font::get_glyph(char c) const
{
    // TODO: OOB checking / handling
    return glyphs.find(c)->second;
}
