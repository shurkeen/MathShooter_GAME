#ifndef PAGESWIDGET_H
#define PAGESWIDGET_H

#include "game.h"
#include "server.h"
#include "client.h"
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QMovie>

class MyBaseStackedWidget : public QWidget{
    Q_OBJECT

public:
    MyBaseStackedWidget(QWidget* parent = nullptr);
    void MyShow();
    QStackedWidget* widgetsList;
    MyTcpServer* myServer;
    Client* myClient;
    ~MyBaseStackedWidget();
};

class MyFirstPageWidget : public QWidget{
    Q_OBJECT

public:
    MyFirstPageWidget(QWidget* parent = nullptr);
    QVBoxLayout* layoutFirstPage;
    QHBoxLayout* middleLayout;
    QPushButton *createBtn;
    QPushButton *connectBtn;
    ~MyFirstPageWidget();
    constexpr static const QSize SIZE = QSize(500, 500);

signals:
    void CreateServerPage();
    void ConnectToServerPage();
};

class MySecondPageCreateGameWidget : public QWidget{
    Q_OBJECT

public:
    MySecondPageCreateGameWidget(QWidget* parent = 0);
    QPushButton *BtnBack;
    QPushButton *BtnStartServer;
    QPushButton *BtnStartGame;
    QLineEdit* inputPortServer;
    QLabel* addressLabelServer;
    QLabel* countLabelPlayers;
    QHBoxLayout* middleLayout;
    QHBoxLayout* countRightLayout;
    QHBoxLayout* addressAndInputPortLayout;
    QVBoxLayout* layoutSecondPage;
    QVBoxLayout* socketAndCountLayout;
    QVBoxLayout* countAndMainLayout;
    ~MySecondPageCreateGameWidget();
    constexpr static const QSize SIZE = QSize(600, 600);

signals:
    void createBackToFirstPage();
    void createStartGamePage();
    void startListeningServer();
};

class MySecondPageConnectGameWidget : public QWidget{
    Q_OBJECT

public:
    MySecondPageConnectGameWidget(QWidget* parent = 0);
    void setConnectedText();
    void setUnConnectedText();
    void unsetConnectedText();
    void setText(QString);
    QLabel* timeBeforeStartGameLabel;
    QPushButton *BtnBack;
    QPushButton *BtnConnectGame;
    QPushButton *BtnStartGame;
    QLineEdit* ipInput;
    QLineEdit* portInput;
    QHBoxLayout* inputLayout;
    QVBoxLayout* layoutSecondPage;
    QVBoxLayout* connectedAndMainLayout;
    QHBoxLayout* connectedRightLayout;
    QHBoxLayout* middleLayout;
    QHBoxLayout* rightLabelTimeLayout;
    QLabel *connectedFlag;
    ~MySecondPageConnectGameWidget();
    constexpr static const QSize SIZE = QSize(600, 600);

signals:
    void ConnectStartGamePage();
    void ConnectBackToFirstPage();
    void ConnectToHost();
};

class MyThirdPageWaitingServerWidget: public QWidget{
    Q_OBJECT
public:
    MyThirdPageWaitingServerWidget(QWidget* parent = 0);
    ~MyThirdPageWaitingServerWidget();

    constexpr static const QSize SIZE = QSize(600, 600);

    QLabel* countConnectedLabelPlayers;
    QLabel* countStartedLabelPlayers;
    QLabel* movieGifLabel;
    QLabel* timeBeforeStartGameLabel;
    QMovie *movieGif;
    QVBoxLayout* labelPlayersLayout;
    QVBoxLayout* layoutThirdPage;
    QHBoxLayout* rightLabelPlayersLayout;
    QHBoxLayout* rightLabelTimeLayout;
    QHBoxLayout* middleMovieGifLayout;
};

class MyThirdPageWaitingClientWidget: public QWidget{
    Q_OBJECT
public:
    MyThirdPageWaitingClientWidget(QWidget* parent = 0);
    ~MyThirdPageWaitingClientWidget();

    constexpr static const QSize SIZE = QSize(600, 600);

    QLabel* movieGifLabel;
    QLabel* timeBeforeStartGameLabel;
    QMovie *movieGif;
    QVBoxLayout* layoutThirdPage;
    QHBoxLayout* rightLabelTimeLayout;
    QHBoxLayout* middleMovieGifLayout;

};

#endif // PAGESWIDGET_H
