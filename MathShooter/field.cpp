#include "field.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSizePolicy>

Field::Field(QWidget* parent) : QFrame(parent)
{
    setFixedSize(FIELD_WEIGHT, FIELD_HEIGHT);

    fieldFrame = new QFrame(this);
    fieldFrame->setFrameStyle(QFrame::Box);

    fieldPixmapForGraph = new QPixmap(FIELD_WEIGHT, FIELD_HEIGHT);
    fieldPixmapForGraph->fill(Qt::transparent);

    fieldPixmapForStaticObjects = new QPixmap(FIELD_WEIGHT, FIELD_HEIGHT);
    fieldPixmapForStaticObjects->fill(Qt::transparent);

    updateDistFactor();

    inPaintingGraph = 0;
    idxPlayer = 0;
}

void Field::timerEvent(QTimerEvent *)
{
    this->update();
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
    QPainter pain(this);

    { // отрисовка статических объектов
        pain.drawPixmap(0, 0, FIELD_WEIGHT, FIELD_HEIGHT, *fieldPixmapForStaticObjects);
    }

    { // отрисовка графика функции
        if(inPaintingGraph == 1){
            //qDebug() << idxPoint;
            setGroupPointsGraph();
            pain.drawPixmap(0, 0, FIELD_WEIGHT, FIELD_HEIGHT, *fieldPixmapForGraph);
        }
    }

    pain.end();
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
    if(abs((abs(point.first) - X_LENGTH / 2)) < OUTSIDE_EPS || abs((abs(point.second) - Y_LENGTH / 2)) < OUTSIDE_EPS){
        return true;
    }
    return false;
}

bool Field::checkingIntersectionGraphWithPlayers(QPair<double, double> point, int idxPlayer)
{
    double dist = 0;
    dist = distanceBetweenTwoPoints(point.first, point.second, m_players[idxPlayer].getCenterPosDekartX(), m_players[idxPlayer].getCenterPosDekartY());
    if(dist < Player::M_RADIUS + KILL_EPS){
        return true;
    }
    return false;
}

bool Field::checkingIntersectionGraphWithObstracles(QPair<double, double> point)
{
    double dist = 0;
    for(int i = 0; i < indestructibleObject.size(); i++){
        dist = distanceBetweenTwoPoints(point.first, point.second, indestructibleObject[i].getCenterPosDekartX(), indestructibleObject[i].getCenterPosDekartY());
        if(dist < indestructibleObject[i].getRadius()){
            return true;
        }
    }
    return false;
}

bool Field::checkingIntersectionPlayersWithObstracle(double X0, double Y0, double R0)
{
    double dist = 0;
    for(int i = 0; i < m_players.size(); i++){
        dist = distanceBetweenTwoPoints(X0, Y0, m_players[i].getCenterPosDekartX(), m_players[i].getCenterPosDekartY());
        if(dist < Player::M_RADIUS + R0){
            return true;
        }
    }
    return false;
}

bool Field::attemptToKill(QPair<double, double> point)
{
    bool kill_flag = false;
    for(int i = 0; i < m_players.size(); i++){
        if(i != ((idxPlayer - 1 +  m_players.size()) %  m_players.size()) && checkingIntersectionGraphWithPlayers(point, i)){
            for(int j = i; j < m_players.size() - 1; j++){
                qSwap(m_players[j], m_players[j + 1]);
            }
            m_players.resize(m_players.size() - 1);
            idxPlayer = idxPlayer % m_players.size();
            kill_flag = true;
        }
    }
    return kill_flag;
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

    delete fieldPixmapForGraph;
    fieldPixmapForGraph = new QPixmap(FIELD_WEIGHT, FIELD_HEIGHT);
    fieldPixmapForGraph->fill(Qt::transparent);
}

void Field::initPlayers()
{
    initCenterPosForPlayers();
}

void Field::initPlayersFromServer(QVector<Player>& playersFromServer)
{
    m_players = playersFromServer;
    for(int i = 0; i < m_players.size(); i++){
        m_players[i].setCenterPosScreenX(convertX_Axes(m_players[i].getCenterPosDekartX()));
        m_players[i].setCenterPosScreenY(convertY_Axes(m_players[i].getCenterPosDekartY()));
    }
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

void Field::setPixmapStaticObjects()
{
    setDekartSystem();
    setIndestructibleObstracle();
    setPlayers();
}

void Field::setDekartSystem()
{
    QPainter *pain = new QPainter(fieldPixmapForStaticObjects);

    pain->setPen(Qt::black);

    pain->drawLine(0, this->height() / 2, this->width(), this->height() / 2);
    pain->drawLine(this->width() / 2, 0, this->width() / 2, this->height());

    delete pain;
}

void Field::setIndestructibleObstracle()
{
    QPainter *pain = new QPainter(fieldPixmapForStaticObjects);

    pain->setBrush(Qt::black);
    pain->setPen(Qt::black);

    double r = 0;
    for(int i = 0; i < indestructibleObject.size(); i++){
        r = indestructibleObject[i].getRadius();
        r *= 100.0;
        r *= 1.0 / 3.0;
        pain->drawEllipse(indestructibleObject[i].getCenterPosScreenX() - (int)r / 2, indestructibleObject[i].getCenterPosScreenY() - (int)r / 2, r, r);
    }

    delete pain;
}

void Field::setPlayers()
{
    QPainter *pain = new QPainter(fieldPixmapForStaticObjects);

    pain->setPen(QPen(Qt::green,Qt::SolidLine));
    pain->setBrush(Qt::yellow);

  //  double r = 0;
    for(int i = 0; i < m_players.size(); i++){
//        r = Player::M_RADIUS; // TO DOOOOOO!!!!
//        r *= 100;
//        r *= 1.0 / 3.0;
        pain->drawEllipse(m_players[i].getCenterPosScreenX() - 30 / 2, m_players[i].getCenterPosScreenY() - 30 / 2, 30, 30);
    }

    delete pain;
}

void Field::setPointGraph()
{
    QPainter *pain = new QPainter(fieldPixmapForGraph);

    pain->setBrush(Qt::black);
    pain->setPen(QColorConstants::Svg::brown);

    if(attemptToKill(dekartDotsGraph[idxPoint])){
        delete fieldPixmapForStaticObjects;
        fieldPixmapForStaticObjects = new QPixmap(FIELD_WEIGHT, FIELD_HEIGHT);
        fieldPixmapForStaticObjects->fill(Qt::transparent);
        setPixmapStaticObjects();
    }
    pain->drawLine(tempScreenDotsGraph[idxPoint - 1].rx(), tempScreenDotsGraph[idxPoint - 1].ry(),
            tempScreenDotsGraph[idxPoint].rx(), tempScreenDotsGraph[idxPoint].ry());

    delete pain;
}

void Field::setGroupPointsGraph()
{
    if(idxPoint == 0){
        tempScreenDotsGraph.push_back(screenDotsGraph[idxPoint]);
        idxPoint++;
    }
    int koff = (int)(1.0 * idxPoint / dekartDotsGraph.size() * 100);
    for(int i = 1; i < 60 + koff; i++){
        if(idxPoint < dekartDotsGraph.size()){
            if(checkingOutside(dekartDotsGraph[idxPoint]) ||
               checkingIntersectionGraphWithObstracles(dekartDotsGraph[idxPoint])){
                endDrawingGraph();
                emit shotTimeEndSignal();
                break;
            }
            else{
                tempScreenDotsGraph.push_back(screenDotsGraph[idxPoint]);
                setPointGraph();
                idxPoint++;
            }
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
    setNextPlayer();
}

void Field::setNextPlayer()
{
    idxPlayer = (idxPlayer + 1) % m_players.size();
}

int Field::getNumberOfPlayers()
{
    return m_players.size();
}

QVector<Player> Field::getPlayers()
{
    return m_players;
}

QVector<Obstacle> Field::getPlayersIndestructibleObject()
{
    return indestructibleObject;
}

void Field::updateCountPlayersAndInitStaticObjects(QVector<Obstacle> m_obstacles, QVector<Player> m_players)
{
    m_players.resize(m_players.size());

    initPlayersFromServer(m_players);
    initObstacleFromServer(m_obstacles);
    setPixmapStaticObjects();
}

void Field::updateCountPlayers(int countPlayers)
{
    m_players.resize(countPlayers);

    initPlayers();
    initIndestructibleObstracle();
    setPixmapStaticObjects();
}

void Field::updateCoordGraph(const QVector<QPair<double, double>>& m_dots)
{
    this->dekartDotsGraph.clear();
    double tmpDekartAxisX = 0;
    double tmpDekartAxisY = 0;
    double diff = differenceBetweenY0andYi(m_dots);
    int X0 = m_players[idxPlayer].getCenterPosDekartX();
    int Y0 = m_players[idxPlayer].getCenterPosDekartY();
    for(const auto& i : m_dots){
        tmpDekartAxisX = i.first;
        tmpDekartAxisY = i.second + diff;
        if((m_players[idxPlayer].getCoordinateQuarter() == 2 || m_players[idxPlayer].getCoordinateQuarter() == 3)
                && tmpDekartAxisX < X0){
            if(distanceBetweenTwoPoints(tmpDekartAxisX, tmpDekartAxisY, X0, Y0) - Player::M_RADIUS > DIFF_GRAPH_PLAYER_EPS){
                this->dekartDotsGraph.push_back(QPair<double, double>(tmpDekartAxisX, tmpDekartAxisY));
            }
            else{
                break;
            }
        }
        if((m_players[idxPlayer].getCoordinateQuarter() == 1 || m_players[idxPlayer].getCoordinateQuarter() == 4)
                && tmpDekartAxisX > X0){
            if(distanceBetweenTwoPoints(tmpDekartAxisX, tmpDekartAxisY, X0, Y0) - Player::M_RADIUS > DIFF_GRAPH_PLAYER_EPS){
                this->dekartDotsGraph.push_back(QPair<double, double>(tmpDekartAxisX, tmpDekartAxisY));
            }
            else{
                this->dekartDotsGraph.clear();
            }
            this->dekartDotsGraph.push_back(QPair<double, double>(tmpDekartAxisX, tmpDekartAxisY));
        }
    }

    if(m_players[idxPlayer].getCoordinateQuarter() == 2 || m_players[idxPlayer].getCoordinateQuarter() == 3){
        for(int i = 0; i < this->dekartDotsGraph.size() / 2; i++){
            swap(this->dekartDotsGraph[i], this->dekartDotsGraph[this->dekartDotsGraph.size() - 1 - i]);
        }
    }
    qDebug() << "MMM: " << this->dekartDotsGraph.size();
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

void Field::initCenterPosForIndestructibleObstracle()
{
    QTime zerno(0, 0, 0);
    QRandomGenerator generatorDist(zerno.secsTo(QTime::currentTime()));
    QRandomGenerator generatorRadius(zerno.secsTo(QTime::currentTime()));

    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    double dx = 0;
    double dy = 0;
    double angle = 0;
    double centerPosDecartObstracleX = 0;
    double centerPosDecartObstracleY = 0;
    double dist = 0;
    double randPositionInLine = 0;
    double randRadius = 0;
    Obstacle tmpIndestructibleObject(0, 0);
    double tmpX0 = 0;
    double tmpY0= 0 ;
    for(int i = 0; i < m_players.size(); i++){
        for(int j = i + 1; j < m_players.size(); j++){
            x1 = m_players[i].getCenterPosDekartX();
            x2 = m_players[j].getCenterPosDekartX();
            y1 = m_players[i].getCenterPosDekartY();
            y2 = m_players[j].getCenterPosDekartY();

            if(y2 < y1){
                std::swap(y1, y2);
                std::swap(x1, x2);
            }

            dx = x2 - x1;
            dy = y2 - y1;
            angle = fabs(angleTanBetweenDxDyObstracle(dx, dy));

            dist = distanceBetweenTwoPoints(x1, y1, x2, y2);

            randPositionInLine = generatorDist.generateDouble() * dist;
            randRadius = (generatorRadius.generate() % (Obstacle::MAX_RADIUS - Obstacle::MIN_RADIUS + 1)) + Obstacle::MIN_RADIUS;

            tmpX0 = findOffsetXForCenterPosObstracle(randPositionInLine, fabs(dx), angle);
            tmpY0 = tmpX0 * angle;

            if(dx > 0){
                tmpX0 = tmpX0 + x1;
            }
            else{
                tmpX0 = x1 - tmpX0;
            }
            tmpY0 += y1;

            while(checkingIntersectionPlayersWithObstracle(tmpX0, tmpY0, randRadius + Obstacle::EXTRA_EPS_RADIUS)){
                randPositionInLine = generatorDist.generateDouble() * dist;

                tmpX0 = findOffsetXForCenterPosObstracle(randPositionInLine, fabs(dx), angle);
                tmpY0 = tmpX0 * angle;

                if(dx > 0){
                    tmpX0 = tmpX0 + x1;
                }
                else{
                    tmpX0 = x1 - tmpX0;
                }
                tmpY0 += y1;

                randRadius = (generatorRadius.generate() % (Obstacle::MAX_RADIUS - Obstacle::MIN_RADIUS + 1)) + Obstacle::MIN_RADIUS;
            }

            centerPosDecartObstracleX = findOffsetXForCenterPosObstracle(randPositionInLine, fabs(dx), angle);
            centerPosDecartObstracleY = centerPosDecartObstracleX * angle;

            if(dx > 0){
                centerPosDecartObstracleX = centerPosDecartObstracleX + x1;
            }
            else{
                centerPosDecartObstracleX = x1 - centerPosDecartObstracleX;
            }
            centerPosDecartObstracleY += y1;

            tmpIndestructibleObject.setCenterPosDekartX(centerPosDecartObstracleX);
            tmpIndestructibleObject.setCenterPosDekartY(centerPosDecartObstracleY);
            tmpIndestructibleObject.setCenterPosScreenX(convertX_Axes(centerPosDecartObstracleX));
            tmpIndestructibleObject.setCenterPosScreenY(convertY_Axes(centerPosDecartObstracleY));
            tmpIndestructibleObject.setRadius(randRadius);
            indestructibleObject.push_back(tmpIndestructibleObject);
        }
    }
}

void Field::initIndestructibleObstracle()
{
    initCenterPosForIndestructibleObstracle();
}

void Field::initObstacleFromServer(QVector<Obstacle>& obstacleFromServer)
{
    indestructibleObject = obstacleFromServer;
    for(int i = 0; i < indestructibleObject.size(); i++){
        indestructibleObject[i].setCenterPosScreenX(convertX_Axes(indestructibleObject[i].getCenterPosDekartX()));
        indestructibleObject[i].setCenterPosScreenY(convertY_Axes(indestructibleObject[i].getCenterPosDekartY()));
    }
}

double Field::distanceBetweenTwoPoints(double x1, double y1, double x2, double y2)
{
    double dx = (x2 - x1);
    double dy = (y2 - y1);
    return sqrt(dx * dx + dy * dy);
}

double Field::findOffsetXForCenterPosObstracle(double dist, double dx, double angle)
{
    double w = 0;
    while(w < dx){
        if(fabs(w * sqrt(1 + angle * angle) - dist) < 1e-1){
            return w;
        }
        w += 0.05;
    }
    return 0;
}

double Field::angleTanBetweenDxDyObstracle(double dx, double dy)
{
    return dy / dx;
}
