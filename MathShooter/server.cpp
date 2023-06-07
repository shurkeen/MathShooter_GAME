#include "server.h"

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer();

    checkNetworkConfiguration();

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    m_timer = new QTimer(this);

    connect(m_timer, &QTimer::timeout, this, &MyTcpServer::checkNetworkConfiguration);

    m_game = new Game;
    countConnectedPlayers = 0;
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
        mVTcpSocket.clear();
        qDebug() << "server close";
    }
}

void MyTcpServer::initGame()
{
    m_game->initGame();
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

QString MyTcpServer::getServerAddress()
{
    checkNetworkConfiguration();
    return serverAddress;
}

void MyTcpServer::serverInitStartGame()
{
    for(int i = 0; i < mVTcpSocket.size(); i++){
        QByteArray array;
        QDataStream out(&array, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_2);

        QString str = "serverInitStartGame";

        out << str;

        mVTcpSocket[i] -> write(array);
    }

}

void MyTcpServer::slotNewConnection()
{
    mVTcpSocket.push_back(mTcpServer -> nextPendingConnection());
    countConnectedPlayers++;

    //mVTcpSocket[mVTcpSocket.size() - 1]->write("Zdarova zaebal\n");
    initClientStaticObject();

    connect(mVTcpSocket[mVTcpSocket.size() - 1], &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(mVTcpSocket[mVTcpSocket.size() - 1], &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnection);

    emit addNewPlayer();
}

void MyTcpServer::slotServerRead()
{
    for(int from = 0; from < mVTcpSocket.size(); from++){
        while(mVTcpSocket[from] -> bytesAvailable() > 0){
            //read
            QByteArray array;
            QDataStream out(&array, QIODevice::WriteOnly);
            out <<  mVTcpSocket[from] -> readAll();
            //

            //write
            QVector<int> qq;
            int tmp = 0;
            for(int i = 0; i < array.size(); i++){
                if(array.at(i) == ' '){
                    qq.push_back(tmp);
                    tmp = 0;
                }
                else if(array.at(i) >= '0' && array.at(i) <= '9'){
                    tmp = tmp * 10 + (array.at(i) - '0');
                }
            }
            qq.push_back(tmp);
            for(const auto& i : qq){
                qDebug() << i << " ";
            }
            //

            for(int to = 0; to < mVTcpSocket.size(); to++){
                mVTcpSocket[to] -> write(array);
                //mVTcpSocket[to] -> write("16.02.2003Alex\n");
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

    countConnectedPlayers--;

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
            emit updateServerAddress(); // To DOOO!!! обработать его
        }
    }
    qDebug() << serverAddress;
}
