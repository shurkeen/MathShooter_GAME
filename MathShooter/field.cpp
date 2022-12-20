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
        tempScreenDotsGraph.push_back(screenDotsGraph[idxPoint++]);
        if(!endLength() && idxPoint >= 1 && !checkingOutside(dekartDotsGraph[idxPoint - 1])
                && !checkingIntersectionWithPlayers(dekartDotsGraph[idxPoint - 1])){
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

    for(const auto& i : tempScreenDotsGraph){
        m_paint.drawEllipse(i, 1, 1);
    }

    m_paint.end();
}

void Field::playersDrawing()
{
    m_paint.begin(this);

    for(int i = 0; i < m_players.size(); i++){
        for(int j = 1; j < m_players[i].screenDotsPlayer.size(); j++){
            m_paint.drawLine(m_players[i].screenDotsPlayer[j - 1], m_players[i].screenDotsPlayer[j]);
        }
    }

    m_paint.setPen(QPen(Qt::green,Qt::SolidLine));
    m_paint.setBrush(Qt::yellow);
    for(int i = 0; i < m_players.size(); i++){
        m_paint.drawEllipse(m_players[i].centerPosScreenX - 13, m_players[i].centerPosScreenY - 13, 25, 25);
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
    screenDotsGraph.clear();
    for(const auto& i : dekartDotsGraph){
        screenDotsGraph.push_back(QPoint(convertX_Axes(i.first), convertY_Axes(i.second)));
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

bool Field::checkingOutside(QPair<double, double> point)
{
    if(abs(point.first) > X_LENGTH / 2 || abs(point.second) > Y_LENGTH / 2){
        return true;
    }
    return false;
}

bool Field::checkingIntersectionWithPlayers(QPair<double, double> point)
{
    for(int i = 0; i < m_players.size(); i++){
        for(int j = 0; j < m_players[i].dekartDotsPlayer.size(); j++){
            if(fabs(point.first - m_players[i].dekartDotsPlayer[j].first) +
               fabs(point.second - m_players[i].dekartDotsPlayer[j].second) < 1e-4){
                return true;
            }
        }
    }
    return false;
}

bool Field::endLength()
{
    return idxPoint == screenDotsGraph.size();
}

void Field::endDrawingGraph()
{
    killTimer(timerId);
    tempScreenDotsGraph.clear();
}

void Field::initPlayers()
{
    initCenterPosForPlayers();
    initDotsForPlayers();
}

void Field::initCenterPosForPlayers()
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

void Field::initDotsForPlayers()
{
    for(int i = 0; i < 1; i++){
        double x0 = -5;
        double y0 = 0;
        double r = m_players[i].M_RADIUS;
        double w = r;
        double h = 0;
        double tmpX1 = 0;
        double tmpX2 = 0;
        double tmpX3 = 0;
        double tmpX4 = 0;
        double tmpY1 = 0;
        double tmpY2 = 0;
        double tmpY3 = 0;
        double tmpY4 = 0;
        while(w > 0){
            h = qSqrt(r * r - w * w);

            tmpX1 = x0 + w;
            tmpY1 = y0 + h;

            tmpX2 = x0 + w;
            tmpY2 = y0 - h;

            tmpX3 = x0 - w;
            tmpY3 = y0 - h;

            tmpX4 = x0 - w;
            tmpY4 = y0 + h;

            m_players[i].dekartDotsPlayer.push_back(QPair<double, double>(tmpX1, tmpY1));
            m_players[i].dekartDotsPlayer.push_back(QPair<double, double>(tmpX2, tmpY2));
            m_players[i].dekartDotsPlayer.push_back(QPair<double, double>(tmpX3, tmpY3));
            m_players[i].dekartDotsPlayer.push_back(QPair<double, double>(tmpX4, tmpY4));
            m_players[i].screenDotsPlayer.push_back(QPoint(convertX_Axes(tmpX1), convertY_Axes(tmpY1)));
            m_players[i].screenDotsPlayer.push_back(QPoint(convertX_Axes(tmpX2), convertY_Axes(tmpY2)));
            m_players[i].screenDotsPlayer.push_back(QPoint(convertX_Axes(tmpX3), convertY_Axes(tmpY3)));
            m_players[i].screenDotsPlayer.push_back(QPoint(convertX_Axes(tmpX4), convertY_Axes(tmpY4)));
            w -= Player::K_POLYGON;
        }
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
