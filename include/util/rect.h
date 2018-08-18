#ifndef RECT_H
#define RECT_H

template<typename T>
struct Rect
{
	T x, y, w, h;
};

using IntRect = Rect<int>;

#endif
