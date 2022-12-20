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
    { // отрисовка координатных осей
        dekartSystemDrawing();
    }

    { // отрисовка игроков на поле
        playersDrawing();
    }

    { // отрисовка графика функции
        tempLineDotsGraph.push_back(m_dots[idxPoint++]);
        if(!endLength() && idxPoint >= 1 && !checkingOutside(tempLineDotsGraph[idxPoint - 1])){
            graphDrawing();
        }
        else{
            if(idxPoint != 0) endDrawingGraph();
        }
    }
}

void Field::graphDrawing()
{

    m_paint.begin(this);

    m_paint.setBrush(Qt::black);
    m_paint.setPen(Qt::black);

    for(const auto& i : tempLineDotsGraph){
        m_paint.drawEllipse(i, 1, 1);
    }

    m_paint.end();
}

void Field::playersDrawing()
{
    m_paint.begin(this);

    m_paint.setBrush(Qt::black);
    m_paint.setPen(Qt::black);

    for(int i = 0; i < m_players.size(); i++){
        m_paint.drawEllipse(m_players[i].centerPosScreenX, m_players[i].centerPosScreenY, 25, 25);
    }
    m_paint.end();
}

void Field::dekartSystemDrawing()
{
    m_paint.begin(this);

    m_paint.setPen(Qt::black);

    m_paint.drawLine(0, this->height() / 2, this->width(), this->height() / 2);
    m_paint.drawLine(this->width() / 2, 0, this->width() / 2, this->height());

    m_paint.end();
}

void Field::convertToScreenSystem()
{
    m_dots.clear();
    for(const auto& i : dekartDotsGraph){
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
    distFactorForY = height() / Y_LENGTH;
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
    tempLineDotsGraph.clear();
}

void Field::initPlayers()
{
    QTime zerno(0, 0, 0);
    QRandomGenerator generator(zerno.secsTo(QTime::currentTime()));

    int signX = -1;
    int signY = 1;

    for(int i = 0; i < m_players.size(); i++){
        int posDecartX = generator.generate() % (int)(X_LENGTH / 2 - 1) * signX;
        int posDecartY = generator.generate() % (int)(Y_LENGTH / 2 - 1) * signY;

        m_players[i].setCenterPosDekartX(posDecartX);
        m_players[i].setCenterPosDekartY(posDecartY);
        m_players[i].setCenterPosScreenX(convertX_Axes(posDecartX));
        m_players[i].setCenterPosScreenY(convertY_Axes(posDecartY));

        signX *= (-1);
        if((i + 1) % 2 == 0){ signY *= (-1); }
    }


}

void Field::updateCountPlayers(int countPlayers)
{
    m_players.resize(countPlayers);

    initPlayers();
}

void Field::updateCoordGraph(const QVector<QPair<double, double>>& m_dots)
{
    int idx = 0;
    this->dekartDotsGraph.clear();
    this->dekartDotsGraph.resize(m_dots.size());
    for(const auto& i : m_dots){
        this->dekartDotsGraph[idx++] = i;
    }
    convertToScreenSystem();
}

void Field::updateField()
{
    idxPoint = 0;
    timerId = startTimer(DELAY);
}
