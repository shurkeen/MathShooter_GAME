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

signals:
    void successfulConnection();
    void unsuccessfulConnection();
    void myDisconnectionToHost();
    void serverInitStartGame();

public slots:
    void slotReadyRead();
    void connected();
    void disconnected();
    void checkNetworkConfiguration();

private:
    void SendToServer(QString str);

    QTimer* m_timer;
    QTcpSocket *socket;
    QByteArray array;
    Game* m_game;
    QVector<Obstacle> indestructibleObject;
    QVector<Player> m_players;
    bool initializationObstacleFlag;
    bool initializationPlayersFlag;
    bool serverInitStartGameFlag;
    QString clientAddress;
};

#endif // CLIENT_H
