#include <QApplication>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QPalette>

#include <QTcpSocket>
#include <QNetworkInterface>

#include "pageswidget.h"

MyBaseStackedWidget::MyBaseStackedWidget(QWidget* parent) : QWidget(parent){
    widgetsList = new QStackedWidget;

    MyFirstPageWidget *firstWidget = new MyFirstPageWidget;
    MySecondPageCreateGameWidget *secondCreateWidget = new MySecondPageCreateGameWidget;
    MySecondPageConnectGameWidget *secondConnectWidget = new MySecondPageConnectGameWidget;
    MyThirdPageWaitingServerWidget *thirdWaitingServerWidget = new MyThirdPageWaitingServerWidget;
    MyThirdPageWaitingClientWidget *thirdWaitingClientWidget = new MyThirdPageWaitingClientWidget;

    myServer = new MyTcpServer;
    myClient = new Client;

    connect(firstWidget, &MyFirstPageWidget::CreateServerPage, this, [this, secondCreateWidget]{widgetsList->setFixedSize(MySecondPageCreateGameWidget::SIZE); secondCreateWidget->addressLabelServer->setText(myServer->getServerAddress()); myServer->startTimerChangeAddressServer(); this->widgetsList->setCurrentWidget(secondCreateWidget);});
    connect(firstWidget, &MyFirstPageWidget::ConnectToServerPage, this, [this, secondConnectWidget]{widgetsList->setFixedSize(MySecondPageConnectGameWidget::SIZE); this->widgetsList->setCurrentWidget(secondConnectWidget);});
    connect(secondCreateWidget, &MySecondPageCreateGameWidget::createBackToFirstPage,  this, [this, firstWidget, secondCreateWidget]{widgetsList->setFixedSize(MyFirstPageWidget::SIZE); secondCreateWidget->BtnStartGame->setEnabled(false); myServer->endTimerChangeAddressServer(); myServer->serverEndListen(); secondCreateWidget->BtnStartServer->setText("Run server"); secondCreateWidget->BtnStartServer->setEnabled(true); secondCreateWidget->inputPortServer->setReadOnly(false); secondCreateWidget->BtnStartServer->setStyleSheet("QPushButton{border: 1px solid rgb(129, 150, 139); background-color: rgba(255, 255, 255, 1); border-radius: 6px; padding:1px 14px 2px 14px; color:black;}"); this->widgetsList->setCurrentWidget(firstWidget);});
    connect(secondConnectWidget, &MySecondPageConnectGameWidget::ConnectBackToFirstPage,  this, [this, firstWidget, secondConnectWidget]{widgetsList->setFixedSize(MyFirstPageWidget::SIZE); myClient->disconnectToTheHost(); secondConnectWidget->ipInput->setReadOnly(false); secondConnectWidget->portInput->setReadOnly(false); this->widgetsList->setCurrentWidget(firstWidget);});
    connect(secondCreateWidget, &MySecondPageCreateGameWidget::createStartGamePage,  this, [this, thirdWaitingServerWidget]{widgetsList->setFixedSize(MyThirdPageWaitingServerWidget::SIZE); thirdWaitingServerWidget->countConnectedLabelPlayers->setText("Count connected players: " + QString::number(myServer->getCountConnectedPlayers())); myServer->stopNewConnectionOnServer(); myServer->serverInitStartGame(); this->widgetsList->setCurrentWidget(thirdWaitingServerWidget);});
    connect(secondCreateWidget, &MySecondPageCreateGameWidget::startListeningServer,  this, [this, secondCreateWidget]{ myServer->serverStartListen(secondCreateWidget->inputPortServer->text().toInt()); });
    connect(secondConnectWidget, &MySecondPageConnectGameWidget::ConnectStartGamePage,  this, [this, thirdWaitingClientWidget]{widgetsList->setFixedSize(MyThirdPageWaitingClientWidget::SIZE); myClient->sendToServerAddClient(); this->widgetsList->setCurrentWidget(thirdWaitingClientWidget);});
    connect(secondConnectWidget, &MySecondPageConnectGameWidget::ConnectToHost,  this, [this, secondConnectWidget]{myClient->connectToTheHost(secondConnectWidget->ipInput->text(), secondConnectWidget->portInput->text().toInt());});
    connect(myServer, &MyTcpServer::addNewPlayer, this, [this, secondCreateWidget]{ secondCreateWidget->countLabelPlayers->setText("Count connected players: " + QString::number(myServer->getCountConnectedPlayers())); });
    connect(myServer, &MyTcpServer::reducePlayer, this, [this, secondCreateWidget]{ secondCreateWidget->countLabelPlayers->setText("Count connected players: " + QString::number(myServer->getCountConnectedPlayers())); });
    connect(myServer, &MyTcpServer::reducePlayer, this, [this, thirdWaitingServerWidget]{ thirdWaitingServerWidget->countConnectedLabelPlayers->setText("Count connected players: " + QString::number(myServer->getCountConnectedPlayers())); thirdWaitingServerWidget->countStartedLabelPlayers->setText("Count started players: " + QString::number(myServer->getCountStartedPlayers())); });
    connect(myServer, &MyTcpServer::serverStarted, this, [secondCreateWidget]{ secondCreateWidget->BtnStartServer->setText("Server is started"); secondCreateWidget->BtnStartServer->setEnabled(false); secondCreateWidget->BtnStartGame->setEnabled(true); secondCreateWidget->inputPortServer->setReadOnly(true); secondCreateWidget->BtnStartServer->setStyleSheet("QPushButton{border: 1px solid rgb(129, 150, 139); background-color: rgba(24, 222, 110, 0.65); border-radius: 6px; padding:1px 14px 2px 14px; color:black;}"); });
    connect(myServer, &MyTcpServer::serverNotStarted, this, [secondCreateWidget]{ secondCreateWidget->BtnStartServer->setText("Enter another port [0; 65535]"); secondCreateWidget->BtnStartServer->setStyleSheet("QPushButton{border: 1px solid rgb(129, 150, 139); background-color: rgba(219, 26, 46, 0.65); border-radius: 6px; padding:1px 14px 2px 14px; color:black;}"); });
    connect(myServer, &MyTcpServer::updateServerAddress, this, [this, secondCreateWidget]{ widgetsList->setFixedSize(MySecondPageCreateGameWidget::SIZE); secondCreateWidget->addressLabelServer->setText(myServer->getServerAddress()); secondCreateWidget->BtnStartGame->setEnabled(false); myServer->serverEndListen(); secondCreateWidget->BtnStartServer->setText("Run server"); secondCreateWidget->BtnStartServer->setEnabled(true); secondCreateWidget->inputPortServer->setReadOnly(false); secondCreateWidget->BtnStartServer->setStyleSheet("QPushButton{border: 1px solid rgb(129, 150, 139); background-color: rgba(255, 255, 255, 1); border-radius: 6px; padding:1px 14px 2px 14px; color:black;}"); this->widgetsList->setCurrentWidget(secondCreateWidget);});
    connect(myServer, &MyTcpServer::getServerNewPlayersStart, this, [this, thirdWaitingServerWidget]{ thirdWaitingServerWidget->countStartedLabelPlayers->setText("Count started players: " + QString::number(myServer->getCountStartedPlayers())); } );
    connect(myServer, &MyTcpServer::signalUpdateTimeStartGame, this, [this, thirdWaitingServerWidget]{ thirdWaitingServerWidget->timeBeforeStartGameLabel->setText("Time before start game: " + QString::number(myServer->getCountSecondsBeforeStartGame())); } );
    connect(myServer, &MyTcpServer::beginToPlay, this, [this]{ widgetsList->setFixedSize(Game::SIZE); this->widgetsList->setCurrentWidget(myServer->getGameWidget());} );
    connect(myClient, &Client::beginToPlay, this, [this]{ widgetsList->setFixedSize(Game::SIZE); this->widgetsList->setCurrentWidget(myClient->getGameWidget());} );
    connect(myClient, &Client::signalUpdateTimeStartGame, this, [this, thirdWaitingClientWidget]{ thirdWaitingClientWidget->timeBeforeStartGameLabel->setText("Time before start game: " + QString::number(myClient->getCountSecondsBeforeStartGame())); } );
    connect(myClient, &Client::signalUpdateTimeStartGame, this, [this, secondConnectWidget]{ secondConnectWidget->timeBeforeStartGameLabel->setText("Time before start game: " + QString::number(myClient->getCountSecondsBeforeStartGame())); } );
    connect(myClient, &Client::successfulConnection,  this, [secondConnectWidget]{ secondConnectWidget->setConnectedText(); secondConnectWidget->ipInput->setReadOnly(true); secondConnectWidget->portInput->setReadOnly(true); });
    connect(myClient, &Client::unsuccessfulConnection,  this, [secondConnectWidget]{ secondConnectWidget->setUnConnectedText();});
    connect(myClient, &Client::myDisconnectionToHost,  this, [this, secondConnectWidget]{ widgetsList->setFixedSize(MySecondPageConnectGameWidget::SIZE); secondConnectWidget->unsetConnectedText(); secondConnectWidget->BtnStartGame->setEnabled(false); secondConnectWidget->timeBeforeStartGameLabel->setText(""); secondConnectWidget->ipInput->setReadOnly(false); secondConnectWidget->portInput->setReadOnly(false); this->widgetsList->setCurrentWidget(secondConnectWidget); });
    connect(myClient, &Client::serverInitStartGame,  this, [secondConnectWidget]{ secondConnectWidget->BtnStartGame->setEnabled(true); });
    connect(myClient, &Client::serverMassageConnectTimeOut,  this, [this, secondConnectWidget]{ myClient->disconnectToTheHost(); secondConnectWidget->connectedFlag->setStyleSheet("color: red;"); secondConnectWidget->setText(QString("Connected time out")); });
    connect(myClient, &Client::countPlayersExceeded,  this, [this, secondConnectWidget]{ myClient->disconnectToTheHost(); secondConnectWidget->connectedFlag->setStyleSheet("color: red;"); secondConnectWidget->setText(QString("Count of players exceeded")); });

    widgetsList->addWidget(firstWidget);
    widgetsList->addWidget(secondCreateWidget);
    widgetsList->addWidget(secondConnectWidget);
    widgetsList->addWidget(thirdWaitingServerWidget);
    widgetsList->addWidget(thirdWaitingClientWidget);
    widgetsList->addWidget(myServer->getGameWidget());
    widgetsList->addWidget(myClient->getGameWidget());
}

void MyBaseStackedWidget::MyShow()
{
    widgetsList->setFixedSize(widgetsList->currentWidget()->size());
    widgetsList->show();
}

MyBaseStackedWidget::~MyBaseStackedWidget(){
    delete widgetsList;
}

MyFirstPageWidget::MyFirstPageWidget(QWidget* parent) : QWidget(parent)
{
    resize(MyFirstPageWidget::SIZE);
    QPalette pal(palette());
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    this->setPalette(pal);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    createBtn = new QPushButton("Create game");
    connectBtn = new QPushButton("Connect to the game");

    layoutFirstPage = new QVBoxLayout();
    layoutFirstPage->addItem(new QSpacerItem(0, 500));
    layoutFirstPage->addWidget(createBtn);
    layoutFirstPage->addWidget(connectBtn);
    layoutFirstPage->addItem(new QSpacerItem(0, 500));

    middleLayout = new QHBoxLayout(this);
    middleLayout->addItem(new QSpacerItem(120, 0));
    middleLayout->addLayout(layoutFirstPage);
    middleLayout->addItem(new QSpacerItem(120, 0));

    connect(createBtn, &QPushButton::clicked, this, [this]{emit CreateServerPage();});
    connect(connectBtn, &QPushButton::clicked, this, [this]{emit ConnectToServerPage();});
}

MyFirstPageWidget::~MyFirstPageWidget()
{
    delete createBtn;
    delete connectBtn;

    delete layoutFirstPage;
    delete middleLayout;
}

MySecondPageCreateGameWidget::MySecondPageCreateGameWidget(QWidget* parent) : QWidget(parent) {
    resize(MySecondPageCreateGameWidget::SIZE);
    QPalette pal(palette());
    pal.setColor(QPalette::Window, QColorConstants::Svg::pink);
    setAutoFillBackground(true);
    this->setPalette(pal);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    BtnBack = new QPushButton("Back");
    BtnStartServer = new QPushButton("Run server");
    BtnStartGame = new QPushButton("Start game");

    BtnStartServer->setStyleSheet("QPushButton{border: 1px solid rgb(129, 150, 139); background-color: rgba(255, 255, 255, 1); border-radius: 6px; padding:1px 14px 2px 14px; color:black;}");
    BtnStartGame->setEnabled(false);

    inputPortServer = new QLineEdit();
    inputPortServer->setPlaceholderText("Port");

    countLabelPlayers = new QLabel("Count connected players: 0");
    addressLabelServer = new QLabel();

    layoutSecondPage = new QVBoxLayout();
    layoutSecondPage->addItem(new QSpacerItem(0, 500));
    layoutSecondPage->addWidget(BtnStartGame);
    layoutSecondPage->addWidget(BtnBack);
    layoutSecondPage->addItem(new QSpacerItem(0, 500));

    middleLayout = new QHBoxLayout();
    middleLayout->addItem(new QSpacerItem(120, 0));
    middleLayout->addLayout(layoutSecondPage);
    middleLayout->addItem(new QSpacerItem(120, 0));

    addressAndInputPortLayout = new QHBoxLayout();
    addressAndInputPortLayout->addWidget(addressLabelServer);
    addressAndInputPortLayout->addWidget(inputPortServer);

    socketAndCountLayout = new QVBoxLayout();
    socketAndCountLayout->addLayout(addressAndInputPortLayout);
    socketAndCountLayout->addWidget(BtnStartServer);
    socketAndCountLayout->addWidget(countLabelPlayers);

    countRightLayout = new QHBoxLayout();
    countRightLayout->addItem(new QSpacerItem(600, 0));
    countRightLayout->addLayout(socketAndCountLayout);

    countAndMainLayout = new QVBoxLayout(this);
    countAndMainLayout->addLayout(countRightLayout);
    countAndMainLayout->addLayout(middleLayout);

    connect(BtnStartServer, &QPushButton::clicked, this, [this]{emit startListeningServer();});
    connect(BtnStartGame, &QPushButton::clicked, this, [this]{emit createStartGamePage();});
    connect(BtnBack, &QPushButton::clicked, this, [this]{emit createBackToFirstPage();});
}

MySecondPageCreateGameWidget::~MySecondPageCreateGameWidget()
{
    delete BtnBack;
    delete BtnStartServer;
    delete BtnStartGame;

    delete layoutSecondPage;
    delete countAndMainLayout;
    delete middleLayout;
    delete countAndMainLayout;
    delete socketAndCountLayout;
    delete addressAndInputPortLayout;
}

MySecondPageConnectGameWidget::MySecondPageConnectGameWidget(QWidget *parent)
{
    resize(MySecondPageCreateGameWidget::SIZE);
    QPalette pal(palette());
    pal.setColor(QPalette::Window, QColorConstants::Svg::pink);
    setAutoFillBackground(true);
    this->setPalette(pal);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    BtnBack = new QPushButton("Back");
    BtnConnectGame = new QPushButton("Connect to host");
    BtnStartGame = new QPushButton("Start game");

    BtnStartGame->setEnabled(false);
    ipInput = new QLineEdit();
    ipInput->setPlaceholderText("Ip address");
    portInput =  new QLineEdit();
    portInput->setFixedWidth(60);
    portInput->setPlaceholderText("Port");

    connectedFlag = new QLabel();
    timeBeforeStartGameLabel = new QLabel();

    rightLabelTimeLayout = new QHBoxLayout();
    rightLabelTimeLayout->addItem(new QSpacerItem(600, 0));
    rightLabelTimeLayout->addWidget(timeBeforeStartGameLabel);

    inputLayout = new QHBoxLayout();
    inputLayout->addWidget(ipInput);
    inputLayout->addWidget(portInput);
    inputLayout->addWidget(BtnConnectGame);

    layoutSecondPage = new QVBoxLayout();
    layoutSecondPage->addItem(new QSpacerItem(0, 500));
    layoutSecondPage->addWidget(BtnStartGame);
    layoutSecondPage->addLayout(inputLayout);
    layoutSecondPage->addWidget(BtnBack);
    layoutSecondPage->addItem(new QSpacerItem(0, 500));

    middleLayout = new QHBoxLayout();
    middleLayout->addItem(new QSpacerItem(120, 0));
    middleLayout->addLayout(layoutSecondPage);
    middleLayout->addItem(new QSpacerItem(120, 0));

    connectedRightLayout = new QHBoxLayout();
    connectedRightLayout->addItem(new QSpacerItem(600, 0));
    connectedRightLayout->addWidget(connectedFlag);

    connectedAndMainLayout =  new QVBoxLayout(this);
    connectedAndMainLayout->addLayout(connectedRightLayout);
    connectedAndMainLayout->addLayout(middleLayout);
    connectedAndMainLayout->addLayout(rightLabelTimeLayout);

    connect(BtnConnectGame, &QPushButton::clicked, this, [this]{emit ConnectToHost();});
    connect(BtnBack, &QPushButton::clicked, this, [this]{emit ConnectBackToFirstPage();});
    connect(BtnStartGame, &QPushButton::clicked, this, [this]{emit ConnectStartGamePage();});
}

void MySecondPageConnectGameWidget::setConnectedText()
{
    connectedFlag->setStyleSheet("color: green;");
    connectedFlag->setText("Connected");
    BtnConnectGame->setEnabled(false);
}

void MySecondPageConnectGameWidget::setUnConnectedText()
{
    connectedFlag->setStyleSheet("color: red;");
    connectedFlag->setText("Not connected");
}

void MySecondPageConnectGameWidget::unsetConnectedText()
{
    connectedFlag->setText("");
    BtnConnectGame->setEnabled(true);
}

void MySecondPageConnectGameWidget::setText(QString str)
{
    connectedFlag->setText(str);
}

MySecondPageConnectGameWidget::~MySecondPageConnectGameWidget()
{
    delete inputLayout;
    delete layoutSecondPage;
    delete middleLayout;
    delete connectedRightLayout;
    delete connectedAndMainLayout;

    delete BtnBack;
    delete BtnConnectGame;
}

MyThirdPageWaitingServerWidget::MyThirdPageWaitingServerWidget(QWidget *parent)
{
    resize(MySecondPageCreateGameWidget::SIZE);
    QPalette pal(palette());
  //  pal.setColor(QPalette::Window, QColorConstants::Svg::greenyellow);
  //  pal.setColor(QPalette::Window, QColorConstants::Svg::black);
    setAutoFillBackground(true);
    this->setPalette(pal);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    countConnectedLabelPlayers = new QLabel();
    countStartedLabelPlayers = new QLabel("Count started players: 0");
    timeBeforeStartGameLabel =  new QLabel();
    movieGifLabel = new QLabel();

    movieGif = new QMovie(":/gifs/gifs/miniml_loading.gif");
  //  movieGif = new QMovie(":/gifs/gifs/rofl_loading.gif");
    movieGif->setScaledSize(QSize(30, 30));

    movieGifLabel->setMovie(movieGif);
    movieGif->start();

    labelPlayersLayout = new QVBoxLayout();
    labelPlayersLayout->addWidget(countConnectedLabelPlayers);
    labelPlayersLayout->addWidget(countStartedLabelPlayers);

    rightLabelPlayersLayout = new QHBoxLayout();
    rightLabelPlayersLayout->addItem(new QSpacerItem(600, 0));
    rightLabelPlayersLayout->addLayout(labelPlayersLayout);

    middleMovieGifLayout =  new QHBoxLayout();
    middleMovieGifLayout->addItem(new QSpacerItem(500, 0));
    middleMovieGifLayout->addWidget(movieGifLabel);
    middleMovieGifLayout->addItem(new QSpacerItem(500, 0));

    rightLabelTimeLayout = new QHBoxLayout();
    rightLabelTimeLayout->addItem(new QSpacerItem(600, 0));
    rightLabelTimeLayout->addWidget(timeBeforeStartGameLabel);

    layoutThirdPage = new QVBoxLayout(this);
    layoutThirdPage->addLayout(rightLabelPlayersLayout);
    layoutThirdPage->addItem(new QSpacerItem(0, 500));
    layoutThirdPage->addLayout(middleMovieGifLayout);
    layoutThirdPage->addItem(new QSpacerItem(0, 500));
    layoutThirdPage->addLayout(rightLabelTimeLayout);
}

MyThirdPageWaitingServerWidget::~MyThirdPageWaitingServerWidget()
{
    delete labelPlayersLayout;
    delete rightLabelPlayersLayout;
}

MyThirdPageWaitingClientWidget::MyThirdPageWaitingClientWidget(QWidget *parent)
{
    resize(MySecondPageCreateGameWidget::SIZE);
    QPalette pal(palette());
    //pal.setColor(QPalette::Window, QColorConstants::Svg::greenyellow);
    setAutoFillBackground(true);
    this->setPalette(pal);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    timeBeforeStartGameLabel =  new QLabel();
    movieGifLabel = new QLabel();

    movieGif = new QMovie(":/gifs/gifs/miniml_loading.gif");
  //  movieGif = new QMovie(":/gifs/gifs/rofl_loading.gif");
    movieGif->setScaledSize(QSize(30, 30));

    movieGifLabel->setMovie(movieGif);
    movieGif->start();

    middleMovieGifLayout =  new QHBoxLayout();
    middleMovieGifLayout->addItem(new QSpacerItem(500, 0));
    middleMovieGifLayout->addWidget(movieGifLabel);
    middleMovieGifLayout->addItem(new QSpacerItem(500, 0));

    rightLabelTimeLayout = new QHBoxLayout();
    rightLabelTimeLayout->addItem(new QSpacerItem(600, 0));
    rightLabelTimeLayout->addWidget(timeBeforeStartGameLabel);

    layoutThirdPage = new QVBoxLayout(this);
    layoutThirdPage->addItem(new QSpacerItem(0, 500));
    layoutThirdPage->addLayout(middleMovieGifLayout);
    layoutThirdPage->addItem(new QSpacerItem(0, 500));
    layoutThirdPage->addLayout(rightLabelTimeLayout);

}

MyThirdPageWaitingClientWidget::~MyThirdPageWaitingClientWidget()
{

}
