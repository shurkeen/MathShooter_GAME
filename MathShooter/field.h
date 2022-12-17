#ifndef FIELD_H
#define FIELD_H

#include <QFrame>
#include <QPainter>
#include <QSignalMapper>

class Field : public QFrame
{
    Q_OBJECT

public:
    Field(QWidget* parent = 0);
    void updateField();

public slots:
    void updateCoordGraph(QVector<QPoint> m_dots);

protected:
    void timerEvent(QTimerEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private:
    static const int DELAY = 5;

    int timerId;
    int idxPoint;

    void doDrawing();
    void graphDrawing();
    void dekartSystemDrawing();

    QVector<QPoint> m_dots;
    QVector<QPoint> tempLineDots;
    QVector<QPoint> dekartDots;
    QFrame* fieldFrame;
    QPainter paint;
};

#endif // FIELD_H
