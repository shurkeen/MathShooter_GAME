#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "game.h"

class Client : public QWidget
{
    Q_OBJECT
public:
    Client();

public slots:
    void slotReadyRead();

private:
    void SendToServer(QString str);

    QTcpSocket *socket;
    QByteArray Data;

    QStackedWidget* stackedWidget;
    QWidget *lobby;
    Game *game;

};

#endif // CLIENT_H
