#ifndef UTIL_H
#define UTIL_H

#include <string>

struct Vec3f
{
    float r;
    float g;
    float b;
};

struct Vec2i
{
    int x;
    int y;
};

void message_box(const std::string& title, const std::string& msg);
Vec3f rgb_to_vec(int color);
[[noreturn]] void error(const std::string& msg);

#endif // UTIL_H
