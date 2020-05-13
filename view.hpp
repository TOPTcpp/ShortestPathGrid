#pragma once
#include <QGraphicsView>
#include <QWidget>
#include <QtWidgets>

class View : public QGraphicsView
{
public:
    View(QWidget* parent = nullptr);
protected:
    virtual void wheelEvent(QWheelEvent *event) override;
private:
    const double scale_factor = 1.1;
};
