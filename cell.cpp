#include "cell.hpp"

void Cell::init()
{
    switch(m_Type){
    case Type::blocked:
        m_Color = Qt::lightGray;
        break;
    case Type::opened:
        m_Color = Qt::white;
        break;
    default:
        break;
    }
    QGraphicsItem::setFlag(ItemIsSelectable);
}

Cell::Cell(const QRectF &rectf, const size_t id, const Type &type)
    : m_Rectf(rectf), m_Id(id), m_Type(type)
{
    init();
}

Cell::Cell(const QPoint &point, const QSize &size,const size_t id, const Type& type)
    : m_Rectf(point, size), m_Id(id), m_Type(type)
{
    init();
}

Cell::Cell(const int x, const int y, const int w, const int h, const size_t id, const Type& type)
    : m_Rectf(QPoint(x,y), QSize(w,h)), m_Id(id), m_Type(type)
{
    init();
}

Cell::~Cell()
{

}

QRectF Cell::boundingRect() const
{
    return m_Rectf;
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(m_Rectf, m_Color);
    painter->drawRect(m_Rectf);

    if(m_State == State::select)
    {
        const QSize size(m_Rectf.width() / 6, m_Rectf.height() / 6);
        QPainterPath point;
        point.addEllipse(m_Rectf.center(), size.width(), size.height());
        painter->fillPath(point, Qt::red);
    }
        Q_UNUSED(option);
        Q_UNUSED(widget);
}

bool Cell::setSelect()
{
   if(m_Type == Type::blocked)
       return false;

   m_State = State::select;
   return true;
}

bool Cell::setNotSelect()
{
    if(m_Type == Type::blocked)
        return false;

    m_State = State::not_select;
    return true;
}
