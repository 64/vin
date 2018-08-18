#ifndef UTIL_H
#define UTIL_H

#include <string>

struct Vec3f
{
    float r;
    float g;
    float b;
};

void message_box(const std::string& title, const std::string& msg);
Vec3f rgb_to_vec(int color);

#endif // UTIL_H
