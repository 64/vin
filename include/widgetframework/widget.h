//
// Created by alex on 8/18/18.
//

#ifndef VIN_WIDGET_H
#define VIN_WIDGET_H

#include <memory>
#include <utility>

#include "util/vector.h"

class Widget 
{
public:
    virtual void draw() const = 0;
    virtual void update() = 0;

    virtual void on_resize(const Vector2i& size) = 0;
    virtual void on_position_changed(const Vector2i& position) = 0;
    virtual void on_event() = 0; /*TODO: event handling*/

    virtual Vector2i get_position() const = 0;
    virtual Vector2i get_size() const = 0;
};

using WidgetWeakPtr = std::weak_ptr<Widget>;

class WidgetBase 
{
public:
    virtual void draw() const;
    virtual void update();

    virtual void on_resize(const Vector2i& size);
    virtual void on_position_changed(const Vector2i& position);
    virtual void on_event();

    virtual Vector2i get_position() const;
    virtual Vector2i get_size() const;

private:
    Vector2i m_position;
    Vector2i m_size;

    WidgetWeakPtr m_owner;
    WidgetWeakPtr m_children;
};

#endif  // VIN_WIDGET_H
