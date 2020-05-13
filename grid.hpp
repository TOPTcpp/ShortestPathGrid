#pragma once
#include "cell.hpp"
#include "graph.hpp"
#include "shortest_path.hpp"
#include <set>
#include <list>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QGraphicsSceneMouseEvent>

constexpr QSize DEFAULT_SIZE_CELL(40,40);

class Grid final: public QGraphicsScene
{
    Q_OBJECT
public:
    Grid(QObject* parent = nullptr);
    Grid(const QRectF sceneRect, QObject* parent = nullptr);
    Grid(const int x, const int y, const int w, const int h, QObject* parent = nullptr);
    ~Grid() {}

    int width() const { return m_sizeGrid.width(); }
    int height() const { return m_sizeGrid.height(); }
    void clear();

    void build(const int width, const int height, const size_t numb_walls);
    void update(const size_t numb_walls);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    void setSize(const size_t w, const size_t h);
    void setPoint(QPointF position);
    void updatePredecessor();
    void updatePath();
    void updateCells(std::list<size_t>::iterator first, std::list<size_t>::iterator last, QColor color);
    void showPath();
    void hidePath();
    void generationRandomWalls(const size_t count);

private:
    QSize m_sizeCell = DEFAULT_SIZE_CELL;
    QSize m_sizeGrid;
    QList<QGraphicsItem*> m_Cells;

    QPair<Cell*, Cell*> m_selectedPoint;
    size_t m_numbSelectedCell = 0;

    Graph<size_t, size_t> m_Graph;
    Predecessor<size_t> m_Predecessor;
    std::list<size_t> m_Path;
};
