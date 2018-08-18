//
// Created by alex on 8/18/18.
//

#ifndef VIN_WIDGET_H
#define VIN_WIDGET_H

#include <utility>
#include <memory>

class Widget
{
public:
    virtual void draw() = 0;
    virtual void update() = 0;

    virtual void on_resize(int width, int height) = 0;
    virtual void on_position_changed(int x, int y) = 0;
    virtual void on_event() = 0; /*TODO: event handling*/

    virtual std::pair<int, int> get_position() = 0;
    virtual std::pair<int, int> get_size() = 0; /*TODO: use something else than std::pair<>*/
};

using WidgetWeakPtr = std::weak_ptr<Widget>;

class WidgetBase
{
public:
    virtual void draw();
    virtual void update();

    virtual void on_resize(int width, int height);
    virtual void on_position_changed(int x, int y);
    virtual void on_event();

    virtual std::pair<int, int> get_position();
    virtual std::pair<int, int> get_size();
private:
    std::pair<int, int> m_position;
    std::pair<int, int> m_size;

    WidgetWeakPtr m_owner;
    WidgetWeakPtr m_children;
};

#endif //VIN_WIDGET_H
