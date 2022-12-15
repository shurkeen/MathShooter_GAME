#ifndef FIELD_H
#define FIELD_H

#include <QFrame>
#include <QPainter>

class Field : public QFrame
{
public:
    Field(QWidget* parent = 0);
    void updateField();

protected:
    void timerEvent(QTimerEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private:
    QVector<QPoint> dots;
    QVector<QPoint> tempLineDots;
    int idxPoint;
    static const int DELAY = 5;
    int timerId;
    void doDrawing();
    QFrame* fieldFrame;
};

#endif // FIELD_H
