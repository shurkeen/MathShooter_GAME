#include "field.h"

Field::Field(QWidget* parent) : QFrame(parent)
{
    setFixedSize(860, 500);
    fieldFrame = new QFrame(this);
    fieldFrame->setFrameStyle(QFrame::Box);

    updateDistFactor();

    inPaintingGraph = 0;
    idxPlayer = 0;
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
        if(inPaintingGraph == 1){
            qDebug() << idxPoint;
            attemptToKill(dekartDotsGraph[idxPoint]);
            tempScreenDotsGraph.push_back(screenDotsGraph[idxPoint++]);
            if(!endLength() && idxPoint >= 1 && !checkingOutside(dekartDotsGraph[idxPoint - 1])){
                graphDrawing();
            }
            else{
                if(idxPoint != 0) endDrawingGraph();
            }
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

//    for(int i = 0; i < m_players.size(); i++){
//        for(int j = 1; j < m_players[i].screenDotsPlayer.size(); j++){
//            m_paint.drawLine(m_players[i].screenDotsPlayer[j - 1], m_players[i].screenDotsPlayer[j]);
//        }
//    }

    m_paint.setPen(QPen(Qt::green,Qt::SolidLine));
    m_paint.setBrush(Qt::yellow);
    for(int i = 0; i < m_players.size(); i++){
        m_paint.drawEllipse(m_players[i].getCenterPosScreenX() - 13, m_players[i].getCenterPosScreenY() - 13, 25, 25);
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

bool Field::checkingIntersectionWithPlayers(QPair<double, double> point, int idxPlayer)
{
    for(int j = 0; j < m_players[idxPlayer].getSizeDekartDots(); j++){
        if(fabs(point.first - m_players[idxPlayer].getDekardDotsPlayer(j).first) +
           fabs(point.second - m_players[idxPlayer].getDekardDotsPlayer(j).second) < KILL_EPS){
            return true;
        }
    }
    return false;
}

void Field::attemptToKill(QPair<double, double> point)
{
    for(int i = 0; i < m_players.size(); i++){
        if(i != idxPlayer && checkingIntersectionWithPlayers(point, i)){
            auto it = m_players.begin() + i;
            m_players.erase(it);
        }
    }
}

bool Field::endLength()
{
    return idxPoint == screenDotsGraph.size();
}

void Field::endDrawingGraph()
{
    killTimer(timerId);
    inPaintingGraph = 0;
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

        int posDecartX = (generator.generate() % (int)(X_LENGTH / 2 - distanceBetweenPlayerAndAxisX) + distanceBetweenPlayerAndAxisX) * signX;
        int posDecartY = (generator.generate() % (int)(Y_LENGTH / 2 - distanceBetweenPlayerAndAxisY) + distanceBetweenPlayerAndAxisY) * signY;

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
    for(int i = 0; i < m_players.size(); i++){
        double x0 = m_players[i].getCenterPosDekartX();
        double y0 = m_players[i].getCenterPosDekartY();
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

            m_players[i].pushBackInDekartDotsPlayer(QPair<double, double>(tmpX1, tmpY1));
            m_players[i].pushBackInDekartDotsPlayer(QPair<double, double>(tmpX2, tmpY2));
            m_players[i].pushBackInDekartDotsPlayer(QPair<double, double>(tmpX3, tmpY3));
            m_players[i].pushBackInDekartDotsPlayer(QPair<double, double>(tmpX4, tmpY4));
            m_players[i].pushBackInScreenDotsPlayer(QPoint(convertX_Axes(tmpX1), convertY_Axes(tmpY1)));
            m_players[i].pushBackInScreenDotsPlayer(QPoint(convertX_Axes(tmpX2), convertY_Axes(tmpY2)));
            m_players[i].pushBackInScreenDotsPlayer(QPoint(convertX_Axes(tmpX3), convertY_Axes(tmpY3)));
            m_players[i].pushBackInScreenDotsPlayer(QPoint(convertX_Axes(tmpX4), convertY_Axes(tmpY4)));
            w -= Player::K_POLYGON;
        }
    }
}

double Field::differenceBetweenY0andYi(const QVector<QPair<double, double>>& m_dots)
{
    for(int i = 0; i < m_dots.size(); i++){
        if(fabs(m_dots[i].first - m_players[idxPlayer].getCenterPosDekartX()) < DIFF_EPS){
            return m_players[idxPlayer].getCenterPosDekartY() - m_dots[i].second;
        }
    }
    return 0.0;
}

void Field::movePlayer()
{
    updateField();
}

void Field::setNextPlayer()
{
    idxPlayer++;
}

int Field::getNumberOfPlayers()
{
    return m_players.size();
}

void Field::updateCountPlayers(int countPlayers)
{
    m_players.resize(countPlayers);

    initPlayers();
}

void Field::updateCoordGraph(const QVector<QPair<double, double>>& m_dots)
{
    this->dekartDotsGraph.clear();
    double tmpDekartAxisX = 0;
    double tmpDekartAxisY = 0;
    double diff = differenceBetweenY0andYi(m_dots);
    for(const auto& i : m_dots){
        tmpDekartAxisX = i.first;
        tmpDekartAxisY = i.second + diff;
        if((m_players[idxPlayer].getCoordinateQuarter() == 2 || m_players[idxPlayer].getCoordinateQuarter() == 3)
                && tmpDekartAxisX < m_players[idxPlayer].getCenterPosDekartX() - Player::M_RADIUS){
            this->dekartDotsGraph.push_back(QPair<double, double>(tmpDekartAxisX, tmpDekartAxisY));
        }
        if((m_players[idxPlayer].getCoordinateQuarter() == 1 || m_players[idxPlayer].getCoordinateQuarter() == 4)
                && tmpDekartAxisX > m_players[idxPlayer].getCenterPosDekartX() + Player::M_RADIUS){
            this->dekartDotsGraph.push_back(QPair<double, double>(tmpDekartAxisX, tmpDekartAxisY));
        }
    }

    if(m_players[idxPlayer].getCoordinateQuarter() == 2 || m_players[idxPlayer].getCoordinateQuarter() == 3){
        for(int i = 0; i < this->dekartDotsGraph.size() / 2; i++){
            swap(this->dekartDotsGraph[i], this->dekartDotsGraph[this->dekartDotsGraph.size() - 1 - i]);
        }
    }
    convertToScreenSystem();
}

void Field::updateField()
{
    if(inPaintingGraph != 1){
        idxPoint = 0;
        inPaintingGraph = 1;
        timerId = startTimer(DELAY);
    }
}
