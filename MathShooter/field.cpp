#include "field.h"

Field::Field(QWidget* parent) : QFrame(parent)
{
    setFixedSize(860, 500);
    fieldFrame = new QFrame(this);
    fieldFrame->setFrameStyle(QFrame::Box);
}

void Field::timerEvent(QTimerEvent *)
{
    tempLineDots.push_back(m_dots[idxPoint++]);
    if(idxPoint == m_dots.size()){
        killTimer(timerId);
        tempLineDots.clear();
    }
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
}

void Field::doDrawing()
{
    dekartSystemDrawing();
    graphDrawing();
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

void Field::updateCoordGraph(QVector<QPoint> m_dots)
{
    int idx = 0;
    this->m_dots.clear();
    this->m_dots.resize(m_dots.size());
    for(const auto& i : m_dots){
        this->m_dots[idx++] = i;
    }
}

void Field::updateField()
{
    idxPoint = 0;
    timerId = startTimer(DELAY);
}
