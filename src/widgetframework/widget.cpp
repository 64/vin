//
// Created by alex on 8/18/18.
//

#include <widgetframework/widget.h>


void WidgetBase::draw()
{
}

void WidgetBase::update()
{
}

void WidgetBase::on_resize(int width, int height)
{
    m_size = std::make_pair(width, height);
}

void WidgetBase::on_position_changed(int x, int y)
{
    m_position = std::make_pair(x, y);
}

void WidgetBase::on_event()
{
}

std::pair<int, int> WidgetBase::get_position()
{
    return m_position;
}


std::pair<int, int> WidgetBase::get_size()
{
    return m_size;
}
