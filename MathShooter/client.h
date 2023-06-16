#ifndef CLIENT_H
#define CLIENT_H

#include "game.h"
#include <QObject>
#include <QTcpSocket>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QNetworkInterface>
#include <QTimer>

class Client : public QWidget
{
    Q_OBJECT
public:
    Client();
    Game* getGameWidget();
    void connectToTheHost(QString, int);
    void disconnectToTheHost();
    void sendToServerAddClient();
    int getCountSecondsBeforeStartGame();
    int getCountSecondsBeforeMoveNextGame();

signals:
    void successfulConnection();
    void unsuccessfulConnection();
    void myDisconnectionToHost();
    void serverInitStartGame();
    void serverMassageConnectTimeOut();
    void countPlayersExceeded();
    void signalUpdateTimeStartGame();
    void beginToPlay();
    void signalUpdateTimeNextMoveGame();
    void serverPushFireBtn();

public slots:
    void slotReadyRead();
    void connected();
    void disconnected();
    void checkNetworkConfiguration();
    void updateTimeStartGame();
    void sendFunctionToServer_Slot();
    void moveTimeSlot();
    void sendToServerAddClientGettingCoord();

private:

    QTimer* m_timer;
    QTimer* gameStartTimer;
    QTimer* moveTime;
    QTcpSocket *socket;
    QByteArray array;
    Game* m_game;
    QVector<Obstacle> indestructibleObject;
    QVector<Player> m_players;
    bool initializationObstacleFlag;
    bool initializationPlayersFlag;
    bool initializationPlayerIdFlag;
    bool serverInitStartGameFlag;
    int countSecondBeforeStartGame;
    QString clientAddress;
    int idPlayer;
    int countSecondBeforeNextMoveGame;
    static constexpr int TIME_WAITING_START_GAME = 1;
    static constexpr int TIME_MOVING = 10;
};

#endif // CLIENT_H
