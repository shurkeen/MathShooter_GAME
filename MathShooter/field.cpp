#include "field.h"

Field::Field(QWidget* parent) : QFrame(parent)
{
    setFixedSize(900, 500);
    fieldFrame = new QFrame(this);
    fieldFrame->setFrameStyle(QFrame::Box);

    dots.resize(1000);
    for(int i = 0; i < 1000; i++){
        dots[i].rx() = i;
        dots[i].ry() = i;
    }
}

void Field::timerEvent(QTimerEvent *)
{
    tempLineDots.push_back(dots[idxPoint++]);
    if(idxPoint == 1000){
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
    QPainter paint(this);
    paint.setBrush(Qt::black);
    paint.setPen(Qt::black);

    for(const auto& i : tempLineDots){
        paint.drawEllipse(i, 1, 1);
    }
}

void Field::updateField()
{
    idxPoint = 0;
    timerId = startTimer(DELAY);
}
