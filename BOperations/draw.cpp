#include "draw.h"
#include <fstream>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;
    /*
    QPointFB p1(0,0);
    QPointFB p2(100,0);
    QPointFB p3(0,100);

    polA.push_back(p1);
    polA.push_back(p2);
    polA.push_back(p3);

    QPointFB p4(50,10);
    QPointFB p5(150,10);
    QPointFB p6(50,110);

    polB.push_back(p4);
    polB.push_back(p5);
    polB.push_back(p6);
    */
}

void Draw::loadPoints(std::string pointsA_path, std::string pointsB_path, QSizeF &canvas_size)
{
    std::ifstream pointsA_file;
    std::ifstream pointsB_file;

    pointsA_file.open(pointsA_path);
    pointsB_file.open(pointsB_path);

    if(!pointsA_file.is_open() && !pointsB_file.is_open())
    {
        return;
    }

    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::min();
    double max_y = std::numeric_limits<double>::min();

    if(pointsA_file.is_open())
    {
        polA.clear();
        double x, y;
        while (pointsA_file >> x >> y)
        {
            polA.push_back(QPointFB(x, y));
            if(x < min_x) min_x = x;
            if(x > max_x) max_x = x;
            if(y < min_y) min_y = y;
            if(y > max_y) max_y = y;
        }
        pointsA_file.close();
    }

    if(pointsB_file.is_open())
    {
        polB.clear();
        double x, y;
        while (pointsB_file >> x >> y)
        {
            polB.push_back(QPointFB(x, y));
            if(x < min_x) min_x = x;
            if(x > max_x) max_x = x;
            if(y < min_y) min_y = y;
            if(y > max_y) max_y = y;
        }
        pointsB_file.close();
    }

    double h = canvas_size.height() - 50;
    double w = canvas_size.width() - 50;

    double xc = w/(max_x-min_x);
    double yc = h/(max_y-min_y);

    for(unsigned int i = 0; i < polA.size(); i++)
    {
        polA[i].setX(((polA[i].x()-min_x)*xc)+25);
        polA[i].setY(((polA[i].y()-min_y)*yc)+25);
    }

    for(unsigned int i = 0; i < polB.size(); i++)
    {
        polB[i].setX(((polB[i].x()-min_x)*xc)+25);
        polB[i].setY(((polB[i].y()-min_y)*yc)+25);
    }
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPainter painter (this);

    painter.begin(this);

    //Draw polygon A
    painter.setPen(Qt::green);
    drawPol(polA, painter);

    //Draw polygon B
    painter.setPen(Qt::blue);
    drawPol(polB, painter);

    //Draw result
    painter.setPen(Qt::red);
    for(std::vector<QPointFB> vec: res)
    {
        drawPol(vec, painter);
    }

    //Draw offset
    painter.setPen(Qt::cyan);
    for(std::vector<QPointFB> b: buff)
    {
        drawPol(b, painter);
    }
}

void Draw::drawPol(std::vector<QPointFB> &pol, QPainter &painter)
{
    QPolygon pol_q;

    for(QPointFB p: pol)
    {
        QPoint pb (p.x(), p.y());
        pol_q.append(pb);
    }

    painter.drawPolygon(pol_q);
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    QPointFB p(e->x(), e->y());

    //Add to polA
    if(ab)
    {
        polA.push_back(p);
    }

    //Add to polB
    else
    {
        polB.push_back(p);
    }

    repaint();
}

void Draw::clearAll()
{
    // Clear all in canvas
    polA.clear();
    polB.clear();
    res.clear();
}

void Draw::clearResults()
{
    // Clear results in canvas
    res.clear();
}
