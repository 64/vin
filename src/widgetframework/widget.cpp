//
// Created by alex on 8/18/18.
//

#include <widgetframework/widget.h>

void WidgetBase::draw() const
{
}

void WidgetBase::update()
{
}

void WidgetBase::on_resize(const Vector2i& size)
{
    m_size = size;
}

void WidgetBase::on_position_changed(const Vector2i& position)
{
    m_position = position;
}

void WidgetBase::on_event()
{
}

Vector2i WidgetBase::get_position() const
{
    return m_position;
}


Vector2i WidgetBase::get_size() const
{
    return m_size;
}
