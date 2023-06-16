#include "client.h"

Client::Client()
{
    m_game = new Game;

    initializationObstacleFlag = false;
    initializationPlayersFlag = false;
    serverInitStartGameFlag = false;
    initializationPlayerIdFlag = false;

    countSecondBeforeStartGame = 0;

    { // обработка сокетов
        socket = new QTcpSocket(this);

        connect(socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
        connect(socket, &QTcpSocket::connected, this, &Client::connected);
        connect(socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    }

    { // обработка вспомогательных данных
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

            sendFunctionToServer_Slot();
        });

        connect(this, &Client::serverPushFireBtn, this, [this]{
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

        connect(m_game, &Game::readyGettingCoord, this, &Client::sendToServerAddClientGettingCoord);

        connect(m_timer, &QTimer::timeout, this, &Client::checkNetworkConfiguration);
        connect(gameStartTimer, &QTimer::timeout, this, &Client::updateTimeStartGame);
        connect(moveTime, &QTimer::timeout, this, &Client::moveTimeSlot);
    }
}

Game *Client::getGameWidget()
{
    return m_game;
}

void Client::connectToTheHost(QString ip, int port)
{
    //m_game->initGame();
    socket->connectToHost(ip, port);

    socket->waitForConnected(100);

    if(!(socket->isOpen() && socket->state() == QAbstractSocket::ConnectedState)){
        emit unsuccessfulConnection();
        qDebug() << "NOT START TIMER";
    }
    else{
        checkNetworkConfiguration();
        qDebug() << "START TIMER";
        m_timer->start(3000);
    }
}

void Client::disconnectToTheHost()
{
    m_timer->stop();
    gameStartTimer->stop();
    qDebug() << "END TIMER'S";

    initializationObstacleFlag = false;
    initializationPlayersFlag = false;
    serverInitStartGameFlag = false;
    initializationPlayerIdFlag = false;

    countSecondBeforeStartGame = 0;
    countSecondBeforeNextMoveGame = 0;

    socket->disconnectFromHost();
}

void Client::sendToServerAddClient()
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    QString str = "StartNewClient";

    out << str;

    socket -> write(array);
}

int Client::getCountSecondsBeforeStartGame()
{
    return Client::TIME_WAITING_START_GAME - countSecondBeforeStartGame;
}

int Client::getCountSecondsBeforeMoveNextGame()
{
    return Client::TIME_MOVING - countSecondBeforeNextMoveGame;
}

void Client::slotReadyRead()
{
    QDataStream input(socket);
    input.setVersion(QDataStream::Qt_6_2);

    if(input.status() == QDataStream::Ok){
        if(serverInitStartGameFlag == false){
            QString str;
            input >> str;

            qDebug() << "STR: " << str;
            if(str == "serverInitStartGame"){
                serverInitStartGameFlag = true;
                gameStartTimer->start(1000);
                emit serverInitStartGame();
            }
            else if(str == "connection timed out"){
                emit serverMassageConnectTimeOut();
            }
            else if(str == "number of players exceeded"){
                emit countPlayersExceeded();
            }
        }
        else if(initializationObstacleFlag == false){
            qDebug() << "ZZZZZZZ";

            QVector<QPair<QPair<double, double>, double> > vecPairIndestructibleObject;
            input >> vecPairIndestructibleObject;

            qDebug() << "SIZE: " << vecPairIndestructibleObject.size();

            for(int i = 0; i < vecPairIndestructibleObject.size(); i++){
                Obstacle tmpObstacle(vecPairIndestructibleObject[i].first.first, vecPairIndestructibleObject[i].first.second);
                tmpObstacle.setRadius(vecPairIndestructibleObject[i].second);
                indestructibleObject.push_back(tmpObstacle);
            }

            initializationObstacleFlag = true;

        }
        else if(initializationPlayersFlag == false){
            qDebug() << "MMMMMMMMMM";

            QVector<QPair<int, int> > vecPairPlayers;
            input >> vecPairPlayers;

            qDebug() << "SIZE: " << vecPairPlayers.size();

            for(int i = 0; i < vecPairPlayers.size(); i++){
                Player tmpPlayer;
                tmpPlayer.setCenterPosDekartX(vecPairPlayers[i].first);
                tmpPlayer.setCenterPosDekartY(vecPairPlayers[i].second);
                m_players.push_back(tmpPlayer);
            }

            initializationPlayersFlag = true;

            m_game->initGame(indestructibleObject, m_players);
        }
        else if(initializationPlayerIdFlag == false){
            qDebug() << "IIIIIIIIIII";

            int idPlayer;

            input >> idPlayer;

            this->idPlayer = idPlayer;

            initializationPlayerIdFlag = true;

            if(idPlayer == 0){
                m_game->unlockFireBtn();
            }

            emit beginToPlay();

            moveTime -> start(1000);
        }
        else{
            QString str;
            input >> str;

            if(str == "Fire Graph"){
                m_game->fireGraph();
            }
            else if(str == "start move time"){
                moveTime -> start(1000);

                idPlayer = (idPlayer + 1) % m_game->getCountPlayers();
                if(idPlayer == 0){
                    m_game->unlockFireBtn();
                }

                this->m_game->setColorTimeText(QColor(0, 0, 0));
            }
            else{
                emit serverPushFireBtn();

                m_game->startTimerGettingCoordForString(str);
            }
        }
    }
    else{
        // написать в нужное поле сообщение об ошибке
    }
}

void Client::connected()
{
    qDebug() << "CONNECTED!!!!!";

    emit successfulConnection();
}

void Client::disconnected()
{
    qDebug() << "DISCONNECTED!!!!!";

    disconnectToTheHost();

    emit myDisconnectionToHost();
}

void Client::checkNetworkConfiguration()
{
    QString newClientAddress = clientAddress;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            newClientAddress =  address.toString();
        }
    }
    if(clientAddress != newClientAddress){
        if(clientAddress != ""){
            disconnectToTheHost();
        }
        clientAddress = newClientAddress;
    }
    qDebug() << clientAddress;
}

void Client::updateTimeStartGame()
{
    if(countSecondBeforeStartGame <= Client::TIME_WAITING_START_GAME){
        emit signalUpdateTimeStartGame();

        countSecondBeforeStartGame++;
    }
    else{
        gameStartTimer->stop();

        countSecondBeforeStartGame = 0;
    }
}

void Client::sendFunctionToServer_Slot()
{
    array.clear();
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    out << m_game->getFireFunction();

    socket->write(array);
    socket->flush();

    socket->waitForBytesWritten();
}

void Client::moveTimeSlot()
{
    if(countSecondBeforeNextMoveGame <= Client::TIME_MOVING){
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

void Client::sendToServerAddClientGettingCoord()
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    QString str = "ClientGettingCoord";

    out << str;

    socket -> write(array);
}
