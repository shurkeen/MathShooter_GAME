#include "server.h"

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer();

    m_game = new Game;
    countConnectedPlayers = 0;
    countStartedPlayers = 0;
    countSecondBeforeStartGame = 0;
    countSecondBeforeNextMoveGame = 0;
    countReadyPlayersFireGraph = 0;
    slotNewConnectionPermit = true;

    checkNetworkConfiguration();

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    m_timer = new QTimer(this);
    gameStartTimer = new QTimer(this);
    moveTime = new QTimer(this);

    connect(m_game, &Game::pushFireBtn, this, [this]{
        moveTime -> stop();

        countSecondBeforeNextMoveGame = 0;

        QColor color(24, 222, 110);
        int brightness = color.toHsv().value();
        brightness *= 0.9;
        color = QColor::fromHsv(color.hue(), color.saturation(), brightness);
        this->m_game->setColorTimeText(color);

        this->m_game->setFireFunction();

        this->m_game->lockFireBtn();

        sendFunctionToClients();
    });

    connect(this, &MyTcpServer::clientPushFireBtn, this, [this]{
        moveTime -> stop();

        countSecondBeforeNextMoveGame = 0;

        QColor color(24, 222, 110);
        int brightness = color.toHsv().value();
        brightness *= 0.9;
        color = QColor::fromHsv(color.hue(), color.saturation(), brightness);
        this->m_game->setColorTimeText(color);
    });

    connect(m_game, &Game::pushFireBtn, this, [this]{
        this->m_game->startTimerGettingCoord();
    });

    connect(m_game, &Game::readyGettingCoord, this, &MyTcpServer::addCountReadyPlayersFireGraph);
    connect(m_timer, &QTimer::timeout, this, &MyTcpServer::checkNetworkConfiguration);
    connect(gameStartTimer, &QTimer::timeout, this, &MyTcpServer::updateTimeStartGame);
    connect(m_game, &Game::shotTimeEndSignal, this, &MyTcpServer::startMoveTimeClientAndServer);
    connect(moveTime, &QTimer::timeout, this, &MyTcpServer::moveTimeSlot);
}

MyTcpServer::~MyTcpServer()
{
    startedGamePlayersSocket.clear();
}

void MyTcpServer::serverStartListen(int port)
{

    if(!mTcpServer -> listen(QHostAddress::Any, port)){
        qDebug() << "server is not started";
        emit serverNotStarted();
    }
    else{
        if(mTcpServer->serverPort() == port){
            qDebug() << "server is started " << port << " " << serverAddress;
            emit serverStarted();
        }
        else{
            qDebug() << "server is not started";
            mTcpServer->close();
            emit serverNotStarted();
        }
    }
}

void MyTcpServer::serverEndListen()
{
    if(mTcpServer && mTcpServer->isListening()){
        mTcpServer->close();
        for(auto& sock : mVTcpSocket){
            qDebug() << "SOCKET: " << sock->localAddress();
            sock->disconnectFromHost();
        }
        for(auto& sock : startedGamePlayersSocket){
            sock->disconnectFromHost();
        }
        mVTcpSocket.clear();
        startedGamePlayersSocket.clear();
        gameStartTimer->stop();
        countSecondBeforeStartGame = 0;
        countSecondBeforeNextMoveGame = 0;
        countReadyPlayersFireGraph = 0;
        slotNewConnectionPermit = true;
        qDebug() << "server close";
    }
}

void MyTcpServer::initGame()
{
    m_game->initGame(startedGamePlayersSocket.size());
}

void MyTcpServer::initClientStaticObject()
{

    { // отправили информацию об объектах
        QByteArray array;
        QDataStream stream(&array, QIODevice::ReadWrite);

        QVector<Obstacle> indestructibleObject = m_game->getPointField()->getPlayersIndestructibleObject();
        QVector<QPair<QPair<double, double>, double> > vecPairindestructibleObject;
        for(int j = 0; j < indestructibleObject.size(); j++){
            double x0 = indestructibleObject[j].getCenterPosDekartX();
            double y0 = indestructibleObject[j].getCenterPosDekartY();
            double r = indestructibleObject[j].getRadius();
            vecPairindestructibleObject.push_back({{x0, y0}, r});
        }

        stream << vecPairindestructibleObject;

        for(int to = 0; to < mVTcpSocket.size(); to++){
            mVTcpSocket[to] -> write(array);
            mVTcpSocket[to] -> flush();
        }
    }

    std::this_thread::sleep_for(std::chrono::nanoseconds(100000000));

    { // отправили информацию об игроках
        QByteArray array;
        QDataStream stream(&array, QIODevice::ReadWrite);

        QVector<Player> m_players = m_game->getPointField()->getPlayers();
        QVector<QPair<int, int> > vecPairPlayers;
        for(int j = 0; j < m_players.size(); j++){
            int x0 = m_players[j].getCenterPosDekartX();
            int y0 = m_players[j].getCenterPosDekartY();
            vecPairPlayers.push_back({x0, y0});
        }

        stream << vecPairPlayers;

        for(int to = 0; to < mVTcpSocket.size(); to++){
            mVTcpSocket[to] -> write(array);
            mVTcpSocket[to] -> flush();
        }
    }

    std::this_thread::sleep_for(std::chrono::nanoseconds(100000000));

    { // отправили информацию об очередности ходов игроков

        QTime zerno(0, 0, 0);
        QRandomGenerator generatorStartId(zerno.secsTo(QTime::currentTime()));
        QVector<int > vecIdPlayers;

        int add = generatorStartId.generate() % (mVTcpSocket.size() + 1);

        idPlayer = add % (mVTcpSocket.size() + 1);

        if(idPlayer == 0){
            m_game->unlockFireBtn();
        }

        for(int j = 0; j < mVTcpSocket.size(); j++){
            vecIdPlayers.push_back((j + add + 1) % (mVTcpSocket.size() + 1));
        }

        for(int to = 0; to < mVTcpSocket.size(); to++){
            QByteArray array;
            QDataStream stream(&array, QIODevice::ReadWrite);

            stream << (int)vecIdPlayers[to];

            mVTcpSocket[to] -> write(array);
            mVTcpSocket[to] -> flush();
        }
    }
}

void MyTcpServer::startTimerChangeAddressServer()
{
    m_timer->start(3000);
}

void MyTcpServer::endTimerChangeAddressServer()
{
    m_timer->stop();
}

QTcpServer *MyTcpServer::getPointTcpServer()
{
    return mTcpServer;
}

Game *MyTcpServer::getGameWidget()
{
    return m_game;
}

int MyTcpServer::getCountConnectedPlayers()
{
    return countConnectedPlayers;
}

int MyTcpServer::getCountStartedPlayers()
{
    return countStartedPlayers;
}

int MyTcpServer::getCountSecondsBeforeStartGame()
{
    return MyTcpServer::TIME_WAITING_START_GAME - countSecondBeforeStartGame;
}

int MyTcpServer::getCountSecondsBeforeMoveNextGame()
{
    return MyTcpServer::TIME_MOVING - countSecondBeforeNextMoveGame;
}

QString MyTcpServer::getServerAddress()
{
    checkNetworkConfiguration();
    return serverAddress;
}

void MyTcpServer::serverInitStartGame()
{
    finishCountConnectedPlayers = countConnectedPlayers;

    for(int i = 0; i < mVTcpSocket.size(); i++){
        QByteArray array;
        QDataStream out(&array, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_2);

        QString str = "serverInitStartGame";

        out << str;

        mVTcpSocket[i] -> write(array);
    }

    gameStartTimer->start(1000);
}

void MyTcpServer::stopNewConnectionOnServer()
{
    qDebug() << "PAUSE";
    slotNewConnectionPermit = false;
}

void MyTcpServer::slotNewConnection()
{
    if(slotNewConnectionPermit == true){
        if(countConnectedPlayers < 4){
            qDebug() << "NEW";
            mVTcpSocket.push_back(mTcpServer -> nextPendingConnection());
            countConnectedPlayers++;

            connect(mVTcpSocket[mVTcpSocket.size() - 1], &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
            connect(mVTcpSocket[mVTcpSocket.size() - 1], &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnection);

            emit addNewPlayer();
        }
        else{
            QTcpSocket* newSock = mTcpServer -> nextPendingConnection();

            QByteArray array;
            QDataStream out(&array, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_2);

            QString str = "number of players exceeded";
            out << str;

            newSock -> write(array);

            newSock->waitForBytesWritten();

            newSock->disconnectFromHost();

            delete newSock;
        }
    }
    else{
        QTcpSocket* newSock = mTcpServer -> nextPendingConnection();

        QByteArray array;
        QDataStream out(&array, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_2);

        QString str = "connection timed out";
        out << str;

        newSock -> write(array);

        newSock->waitForBytesWritten();

        newSock->disconnectFromHost();

        delete newSock;
    }
}

void MyTcpServer::slotServerRead()
{
    for(int from = 0; from < mVTcpSocket.size(); from++){
        while(mVTcpSocket[from] -> bytesAvailable() > 0){
            QDataStream input(mVTcpSocket[from]);
            input.setVersion(QDataStream::Qt_6_2);

            QString str;

            input >> str;

            if(str ==  "StartNewClient"){
                countStartedPlayers++;
                startedGamePlayersSocket.push_back(mVTcpSocket[from]);
                emit getServerNewPlayersStart();
            }
            else if(str == "ClientGettingCoord"){
                addCountReadyPlayersFireGraph();
            }
            else{
                emit clientPushFireBtn();

                {
                    QByteArray array;
                    QDataStream out(&array, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_6_2);

                    out << str;

                    for(int extra_from = 0; extra_from < mVTcpSocket.size(); extra_from++){
                        if(extra_from != from){
                            mVTcpSocket[extra_from] -> write(array);
                            mVTcpSocket[extra_from] -> flush();
                            mVTcpSocket[extra_from] -> waitForBytesWritten(1000);
                        }
                    }
                }

                m_game->startTimerGettingCoordForString(str);
            }
        }
    }
}

void MyTcpServer::slotClientDisconnection()
{
    for(int i = 0; i < mVTcpSocket.size(); i++){
        if(mVTcpSocket[i]->state() == QAbstractSocket::UnconnectedState){
            auto it = mVTcpSocket.begin() + i;
            mVTcpSocket[i]->disconnectFromHost();
            qDebug() << "SOCKET ERASE: " << mVTcpSocket[i]->localAddress();
            mVTcpSocket.erase(it);
            break;
        }
    }

    for(int i = 0; i < startedGamePlayersSocket.size(); i++){
        if(startedGamePlayersSocket[i]->state() == QAbstractSocket::UnconnectedState){
            auto it = startedGamePlayersSocket.begin() + i;
            startedGamePlayersSocket[i]->disconnectFromHost();
            startedGamePlayersSocket.erase(it);
            break;
        }
    }

    countConnectedPlayers--;
    countStartedPlayers = fmax(0, countStartedPlayers - 1);

    emit reducePlayer();
}

void MyTcpServer::checkNetworkConfiguration()
{
    QString newServerAddress = serverAddress;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            newServerAddress =  address.toString();
        }
    }
    if(serverAddress != newServerAddress){
        serverAddress = newServerAddress;
        if(serverAddress != ""){
            serverEndListen();
            emit updateServerAddress();
        }
    }
    qDebug() << serverAddress;
}

void MyTcpServer::updateTimeStartGame()
{
    if(countSecondBeforeStartGame <= MyTcpServer::TIME_WAITING_START_GAME){
        emit signalUpdateTimeStartGame();

        countSecondBeforeStartGame++;
    }
    else{
        gameStartTimer->stop();
        countSecondBeforeStartGame = 0;

        initGame();

        std::sort(mVTcpSocket.begin(), mVTcpSocket.end());
        std::sort(startedGamePlayersSocket.begin(), startedGamePlayersSocket.end());
        for(int i = 0; i < mVTcpSocket.size(); i++){
            if(std::find(startedGamePlayersSocket.begin(), startedGamePlayersSocket.end(), mVTcpSocket[i]) == startedGamePlayersSocket.end()){
                mVTcpSocket[i]->disconnectFromHost();
            }
        }

        mVTcpSocket = startedGamePlayersSocket;
        initClientStaticObject();

        moveTime -> start(1000);

        emit beginToPlay();
    }
}

void MyTcpServer::sendFunctionToClients()
{
    {
        QByteArray array;
        QDataStream out(&array, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_2);

        QString str = m_game->getFireFunction();

        out << str;

        for(int from = 0; from < mVTcpSocket.size(); from++){
            mVTcpSocket[from] -> write(array);
            mVTcpSocket[from] -> flush();
            mVTcpSocket[from] -> waitForBytesWritten(1000);
        }
    }
}

void MyTcpServer::startMoveTimeClientAndServer()
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    QString str = "start move time";

    out << str;

    for(int from = 0; from < mVTcpSocket.size(); from++){
        mVTcpSocket[from] -> write(array);
        mVTcpSocket[from] -> flush();
        mVTcpSocket[from] -> waitForBytesWritten(1000);
    }

    moveTime -> start(1000);

    idPlayer = (idPlayer + 1) % m_game->getCountPlayers();
    if(idPlayer == 0){
        m_game->unlockFireBtn();
    }

    this->m_game->setColorTimeText(QColor(0, 0, 0));
}

void MyTcpServer::moveTimeSlot()
{
    if(countSecondBeforeNextMoveGame <= MyTcpServer::TIME_MOVING){
        emit signalUpdateTimeNextMoveGame();

        countSecondBeforeNextMoveGame++;
    }
    else{
        moveTime -> stop();
        countSecondBeforeNextMoveGame = 0;

        m_game->lockFireBtn();

        idPlayer = (idPlayer + 1) % m_game->getCountPlayers();
        if(idPlayer == 0){
            m_game->unlockFireBtn();
        }

        moveTime -> start(1000);
    }
}

void MyTcpServer::checkCountReadyFireGraph()
{
    qDebug() << "EKLJBFCLJKHEBCVJHEWBFVJHBEV " << countReadyPlayersFireGraph << " " << countStartedPlayers;
    if(countReadyPlayersFireGraph == countStartedPlayers + 1){
        {
            QByteArray arrayGo;
            QDataStream outGo(&arrayGo, QIODevice::WriteOnly);
            outGo.setVersion(QDataStream::Qt_6_2);

            QString strGo = "Fire Graph";

            outGo << strGo;

            for(int from = 0; from < mVTcpSocket.size(); from++){
                mVTcpSocket[from] -> write(arrayGo);
                mVTcpSocket[from] -> flush();
                mVTcpSocket[from] -> waitForBytesWritten(1000);
            }
        }

        this->m_game->fireGraph();

        countReadyPlayersFireGraph = 0;
    }
}

void MyTcpServer::addCountReadyPlayersFireGraph()
{
    countReadyPlayersFireGraph++;
    checkCountReadyFireGraph();
}
