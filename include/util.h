#ifndef UTIL_H
#define UTIL_H

#include <string>

struct Vec3f
{
    Vec3f(float _r, float _g, float _b) : r(_r), g(_g), b(_g) {}
    float r;
    float g;
    float b;
};

struct Vec2i
{
    Vec2i(int _x, int _y) : x(_x), y(_y) {}
    int x;
    int y;
};

void message_box(const std::string& title, const std::string& msg);
Vec3f rgb_to_vec(int color);
[[noreturn]] void error(const std::string& msg);

#endif // UTIL_H
