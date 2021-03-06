#ifndef UTIL_H
#define UTIL_H

#include <string>

struct Vec3f
{
    Vec3f(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
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

template<typename T>
struct Rect
{
	T x, y, w, h;
};

using IntRect = Rect<int>;

void message_box(const std::string& title, const std::string& msg, bool err);
Vec3f rgb_to_vec(int color);
[[noreturn]] void error(const std::string& msg);

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

#endif // UTIL_H
