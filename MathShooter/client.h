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

signals:
    void successfulConnection();
    void unsuccessfulConnection();
    void myDisconnectionToHost();
    void serverInitStartGame();
    void serverMassageConnectTimeOut();
    void countPlayersExceeded();
    void signalUpdateTimeStartGame();
    void beginToPlay();

public slots:
    void slotReadyRead();
    void connected();
    void disconnected();
    void checkNetworkConfiguration();
    void updateTimeStartGame();

private:
    void SendToServer(QString str);

    QTimer* m_timer;
    QTimer* gameStartTimer;
    QTcpSocket *socket;
    QByteArray array;
    Game* m_game;
    QVector<Obstacle> indestructibleObject;
    QVector<Player> m_players;
    bool initializationObstacleFlag;
    bool initializationPlayersFlag;
    bool serverInitStartGameFlag;
    int countSecondBeforeStartGame;
    QString clientAddress;
    static constexpr int TIME_WAITING_START_GAME = 20;
};

#endif // CLIENT_H
