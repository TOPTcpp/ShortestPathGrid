#pragma once
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

class Cell: public QGraphicsItem
{
    void init();
public:
    enum class State {select, not_select};
    enum class Type {opened, blocked};

    Cell(const QRectF& rectf, const size_t id, const Type& type = Type::opened);
    Cell(const QPoint& point, const QSize& size, const size_t id, const Type& type = Type::opened);
    Cell(const int x, const int y, const int w, const int h,const size_t id, const Type& type = Type::opened);
    ~Cell();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    Type getType() const { return m_Type; }
    void setType(Cell::Type type) { m_Type = type; init(); }
    void setColor(QColor color) { m_Color = color; }

    bool setSelect();
    bool setNotSelect();

    size_t id() const { return m_Id; }

private:
    QRectF m_Rectf;
    QColor m_Color;
    size_t m_Id = 0;
    Type m_Type;
    State m_State = State::not_select;
};
