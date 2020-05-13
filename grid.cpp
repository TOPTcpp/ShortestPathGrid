#include "grid.hpp"

Grid::Grid(QObject* parent)
    : QGraphicsScene(parent)
{

}

void Grid::generationRandomWalls(const size_t count)
{
    std::uniform_int_distribution<> distrib(0, Grid::width() * Grid::height() - 1);
    std::set<size_t> walls;

    while(walls.size() != count)
    {
        const size_t value = distrib(*QRandomGenerator::global());

        if(m_selectedPoint.first != nullptr)
        {
            if(m_selectedPoint.first->id() == value)
                continue;
        }
        if(m_selectedPoint.second != nullptr)
        {
            if(m_selectedPoint.second->id() == value)
                continue;
        }

        if(walls.find(value) == walls.end())
            walls.insert(value);
    }

   for(auto& id: walls)
   {
      m_Graph.remove_vertex(id);
      static_cast<Cell*>(m_Cells[id])->setType(Cell::Type::blocked);
   }
}

void Grid::setSize(const size_t w, const size_t h)
{
   m_sizeGrid = QSize(w,h);
   QGraphicsScene::setSceneRect(QRect(0,0,w * m_sizeCell.width(), h * m_sizeCell.height()));
}

void Grid::setPoint(QPointF position)
{
    Cell* cell = static_cast<Cell*>(QGraphicsScene::itemAt(position, QTransform()));

    switch(m_numbSelectedCell)
    {
    case 0:
        if(cell->setSelect())
        {
            m_selectedPoint.first = cell;
            ++m_numbSelectedCell;
            updatePredecessor();
        }
        break;
    case 1:
        if(cell != m_selectedPoint.first)
        {
            if(cell->setSelect())
            {
                m_selectedPoint.second = cell;
                ++m_numbSelectedCell;
                showPath();
            }
        }
        else
        {
            cell->setNotSelect();
            m_selectedPoint.first = nullptr;
            --m_numbSelectedCell;
        }
        break;
    case 2:
        if(cell == m_selectedPoint.second )
        {
            hidePath();
            cell->setNotSelect();
            m_selectedPoint.second  = nullptr;
            --m_numbSelectedCell;
        }
        else if(cell == m_selectedPoint.first)
        {
            hidePath();
            cell->setNotSelect();
            m_selectedPoint.first = nullptr;
            std::swap(m_selectedPoint.first, m_selectedPoint.second);
            --m_numbSelectedCell;
            updatePredecessor();
        }
        break;
    default:
        break;
    }
}

void Grid::clear()
{
    m_Graph.clear();
    QGraphicsScene::clear();
    m_selectedPoint.first = m_selectedPoint.second = nullptr;
    m_numbSelectedCell = 0;
}

void Grid::build(const int width, const int height, const size_t numb_walls)
{
    this->clear();

    setSize(width, height);

    for(int row = 0; row < height; ++row)
    {
        for(int column = 0; column < width; ++column)
        {
            const size_t id = row * width + column;

            // create cell in scene
            QPoint point(column * m_sizeCell.width() , row * m_sizeCell.height());
            QGraphicsScene::addItem(new Cell(point, m_sizeCell, id, Cell::Type::opened));

            // assign graph
            m_Graph.add_vertex(id);
            if(column > 0)
                m_Graph.add_edge(id, id - 1, 1, 1);
            if(row > 0)
                m_Graph.add_edge(id, id - width, 1, 1);
        }
    }

    m_Cells = this->items(Qt::SortOrder::AscendingOrder);
    generationRandomWalls(numb_walls);
    QGraphicsScene::update();
}

void Grid::update(const size_t numb_walls)
{
    this->hidePath();

    std::vector<size_t> block_cells;

    // look for blocked cells and reset from opened
    for(auto& c: m_Cells)
    {
        Cell* cell = static_cast<Cell*>(c);

        if(cell->getType() == Cell::Type::blocked)
        {
            block_cells.push_back(cell->id());
            cell->setType(Cell::Type::opened);
        }
    }

    const size_t width = this->width();
    const size_t height = this->height();

    // restore graph
    for(auto& id: block_cells)
    {
        m_Graph.add_vertex(id);
        const size_t column = id % width;
        const size_t row = (id - column) / height;

        // link left cell
        if(column > 0)
            m_Graph.add_edge(id, id - 1, 1, 1);
        // link right cell
        if(column < width - 1)
            m_Graph.add_edge(id, id + 1, 1, 1);
        // link top cell
        if(row > 0)
            m_Graph.add_edge(id, id - width, 1, 1);
        // link bot cell
        if(row < height - 1)
            m_Graph.add_edge(id, id + width, 1, 1);
    }

    generationRandomWalls(numb_walls);
    updatePredecessor();
    showPath();

    QGraphicsScene::update();
}

void Grid::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::update();
    QGraphicsScene::mousePressEvent(event);

    const QPointF position = event->pos();
    setPoint(position);
}

void Grid::updatePredecessor()
{
    if(m_selectedPoint.first != nullptr)
        m_Predecessor = Shortest_path_unchecked(m_Graph, m_selectedPoint.first->id());
}

void Grid::updatePath()
{
    if(m_selectedPoint.second != nullptr)
        Construct_shortest_path(m_selectedPoint.second->id(), m_Predecessor, m_Path);
}

void Grid::updateCells(std::list<size_t>::iterator first, std::list<size_t>::iterator last, QColor color)
{
    if(std::distance(first, last) > 1)
    {
        while(first != last)
        {
            static_cast<Cell*>(m_Cells[*first])->setColor(color);
            ++first;
        }
    }
}

void Grid::showPath()
{
    updatePath();

    if(m_selectedPoint.second != nullptr)
        updateCells(m_Path.begin(), m_Path.end(), Qt::green);
}

void Grid::hidePath()
{
    updateCells(m_Path.begin(), m_Path.end(), Qt::white);
}
