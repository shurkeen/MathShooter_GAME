#include "client.h"

Client::Client()
{
    m_game = new Game;

    initializationObstacleFlag = false;
    initializationPlayersFlag = false;
    serverInitStartGameFlag = false;

    { // обработка сокетов
        socket = new QTcpSocket(this);
        m_timer = new QTimer(this);

        connect(m_timer, &QTimer::timeout, this, &Client::checkNetworkConfiguration);
        connect(socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
        connect(socket, &QTcpSocket::connected, this, &Client::connected);
        connect(socket, &QTcpSocket::disconnected, this, &Client::disconnected);
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
    qDebug() << "END TIMER";

    initializationObstacleFlag = false;
    initializationPlayersFlag = false;
    serverInitStartGameFlag = false;

    socket->disconnectFromHost();
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
                emit serverInitStartGame();
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
        else{
//            QString str;
//            in >> str;
//            написать установление строки в нужное поле
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

void Client::SendToServer(QString str)
{
    array.clear();
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    socket->write(array);
}
