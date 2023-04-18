#include "client.h"

Client::Client()
{
    { // создание окон(виджетов) приложения
        stackedWidget = new QStackedWidget;
        lobby = new QWidget;
        game = new Game;

        stackedWidget->addWidget(lobby);
        stackedWidget->addWidget(game);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(stackedWidget);
        setLayout(layout);

        game->initGame();

        stackedWidget->setCurrentWidget(game);
        //stackedWidget->setCurrentWidget(lobby);
    }

    { // обработка сокетов
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    }
}

void Client::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok){
        QString str;
        in >> str;
        // написать установление строки в нужное поле
    }
    else{
        // написать в нужное поле сообщение об ошибке
    }
}

void Client::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    socket->write(Data);
}
