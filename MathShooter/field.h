#ifndef FIELD_H
#define FIELD_H

#include <QFrame>
#include <QPainter>
#include <QSignalMapper>
#include <QPair>

class Field : public QFrame
{
    Q_OBJECT

public:
    Field(QWidget* parent = 0);
    void updateField();

public slots:
    void updateCoordGraph(const QVector<QPair<double, double>>& m_dots);

protected:
    void timerEvent(QTimerEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private:
    static const int DELAY = 5;
    constexpr static const double X_LENGTH = 50.0;
    constexpr static const double Y_LENGTH = 30.0;
    double distFactorForX;
    double distFactorForY;

    int timerId;
    int idxPoint;

    void doDrawing();
    void graphDrawing();
    void dekartSystemDrawing();
    void convertToScreenSystem();
    int convertX_Axes(double);
    int convertY_Axes(double);
    void updateDistFactor();
    bool checkingOutside(QPoint point);
    bool endLength();
    void endDrawingGraph();

    QVector<QPoint> m_dots;
    QVector<QPoint> tempLineDots;
    QVector<QPair<double, double>> dekartDots;
    QFrame* fieldFrame;
    QPainter paint;
};

#endif // FIELD_H
