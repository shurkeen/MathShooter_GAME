#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "QtCore/qobject.h"
#include "game.h"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QNetworkInformation>
#include <QVector>
#include <QTimer>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    void serverStartListen(int port);
    void serverEndListen();
    void initGame();
    void initClientStaticObject();
    void startTimerChangeAddressServer();
    void endTimerChangeAddressServer();
    QTcpServer* getPointTcpServer();
    Game* getGameWidget();
    int getCountConnectedPlayers();
    QString getServerAddress();
    void serverInitStartGame();

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnection();
    void checkNetworkConfiguration();

signals:
    void addNewPlayer();
    void reducePlayer();
    void serverStarted();
    void serverNotStarted();
    void updateServerAddress();

private:
    QTcpServer *mTcpServer;
    QVector<QTcpSocket *> mVTcpSocket;
    Game* m_game;
    int countConnectedPlayers;
    QString serverAddress;
    QTimer* m_timer;
};

#endif // MYTCPSERVER_H
