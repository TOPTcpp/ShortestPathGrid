#include "view.hpp"

View::View(QWidget* parent)
    : QGraphicsView(parent)
{

}

void View::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    if(event->delta() > 0)
        scale(scale_factor, scale_factor);
    else
        scale(1/scale_factor, 1/scale_factor);
}
