#include "field.h"

Field::Field(QWidget* parent) : QFrame(parent)
{
    setFixedSize(860, 500);
    fieldFrame = new QFrame(this);
    fieldFrame->setFrameStyle(QFrame::Box);

    updateDistFactor();
}

void Field::timerEvent(QTimerEvent *)
{
    tempLineDots.push_back(m_dots[idxPoint++]);
//    if(idxPoint == m_dots.size()){
//        endDrawingGraph();
//    }
    this->repaint();
}

void Field::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    doDrawing();
}

void Field::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    fieldFrame->resize(size());
    updateDistFactor();
}

void Field::doDrawing()
{
    dekartSystemDrawing();

    { // отрисовка графика функции
        if(!endLength() && idxPoint >= 1 && !checkingOutside(tempLineDots[idxPoint - 1])){
            graphDrawing();
        }
        else{
            if(idxPoint != 0) endDrawingGraph();
        }
    }
}

void Field::graphDrawing()
{

    paint.begin(this);

    paint.setBrush(Qt::black);
    paint.setPen(Qt::black);

    for(const auto& i : tempLineDots){
        paint.drawEllipse(i, 1, 1);
    }

    paint.end();
}

void Field::dekartSystemDrawing()
{
    paint.begin(this);

    paint.setPen(Qt::black);

    paint.drawLine(0, this->height() / 2, this->width(), this->height() / 2);
    paint.drawLine(this->width() / 2, 0, this->width() / 2, this->height());

    paint.end();
}

void Field::convertToScreenSystem()
{
    m_dots.clear();
    for(const auto& i : dekartDots){
        m_dots.push_back(QPoint(convertX_Axes(i.first), convertY_Axes(i.second)));
    }
}

int Field::convertX_Axes(double m_x)
{
    double m_x_screenAxes = width() / 2 + m_x * distFactorForX;
    return m_x_screenAxes;
}

int Field::convertY_Axes(double m_y)
{
    double m_y_screenAxes = height() / 2 - m_y * distFactorForY;
    return m_y_screenAxes;
}

void Field::updateDistFactor()
{
    distFactorForX = width() / X_LENGTH;
    distFactorForY = height() / X_LENGTH;
}

bool Field::checkingOutside(QPoint point)
{
    if(abs(point.rx()) > width() || abs(point.ry()) > height()){
        return true;
    }
    return false;
}

bool Field::endLength()
{
    return idxPoint == m_dots.size();
}

void Field::endDrawingGraph()
{
    killTimer(timerId);
    tempLineDots.clear();
}

void Field::updateCoordGraph(const QVector<QPair<double, double>>& m_dots)
{
    int idx = 0;
    this->dekartDots.clear();
    this->dekartDots.resize(m_dots.size());
    for(const auto& i : m_dots){
        this->dekartDots[idx++] = i;
    }
    convertToScreenSystem();
}

void Field::updateField()
{
    idxPoint = 0;
    timerId = startTimer(DELAY);
}
