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
    ~MyTcpServer();
    void serverStartListen(int port);
    void serverEndListen();
    void initGame();
    void initClientStaticObject();
    void startTimerChangeAddressServer();
    void endTimerChangeAddressServer();
    QTcpServer* getPointTcpServer();
    Game* getGameWidget();
    int getCountConnectedPlayers();
    int getCountStartedPlayers();
    int getCountSecondsBeforeStartGame();
    QString getServerAddress();
    void serverInitStartGame();
    void stopNewConnectionOnServer();

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnection();
    void checkNetworkConfiguration();
    void updateTimeStartGame();

signals:
    void addNewPlayer();
    void reducePlayer();
    void serverStarted();
    void serverNotStarted();
    void updateServerAddress();
    void getServerNewPlayersStart();
    void signalUpdateTimeStartGame();
    void beginToPlay();

private:
    QTcpServer *mTcpServer;
    QVector<QTcpSocket *> mVTcpSocket;
    QVector<QTcpSocket *> startedGamePlayersSocket;
    Game* m_game;
    int countConnectedPlayers;
    int countStartedPlayers;
    int finishCountConnectedPlayers;
    int countSecondBeforeStartGame;
    QString serverAddress;
    QTimer* m_timer;
    QTimer* gameStartTimer;
    bool slotNewConnectionPermit;
    static constexpr int TIME_WAITING_START_GAME = 20;
};

#endif // MYTCPSERVER_H
