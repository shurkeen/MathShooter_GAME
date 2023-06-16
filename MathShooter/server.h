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
    int getCountSecondsBeforeMoveNextGame();
    QString getServerAddress();
    void serverInitStartGame();
    void stopNewConnectionOnServer();

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnection();
    void checkNetworkConfiguration();
    void updateTimeStartGame();
    void sendFunctionToClients();
    void startMoveTimeClientAndServer();
    void moveTimeSlot();
    void checkCountReadyFireGraph();
    void addCountReadyPlayersFireGraph();

signals:
    void addNewPlayer();
    void reducePlayer();
    void serverStarted();
    void serverNotStarted();
    void updateServerAddress();
    void getServerNewPlayersStart();
    void signalUpdateTimeStartGame();
    void beginToPlay();
    void signalUpdateTimeNextMoveGame();
    void clientPushFireBtn();

private:
    QTcpServer *mTcpServer;
    QVector<QTcpSocket *> mVTcpSocket;
    QVector<QTcpSocket *> startedGamePlayersSocket;
    Game* m_game;
    int countConnectedPlayers;
    int countStartedPlayers;
    int finishCountConnectedPlayers;
    int countSecondBeforeStartGame;
    int countSecondBeforeNextMoveGame;
    int countReadyPlayersFireGraph;
    QString serverAddress;
    QTimer* m_timer;
    QTimer* gameStartTimer;
    QTimer* moveTime;
    bool slotNewConnectionPermit;
    int idPlayer;
    static constexpr int TIME_WAITING_START_GAME = 1;
    static constexpr int TIME_MOVING = 10;
};

#endif // MYTCPSERVER_H
