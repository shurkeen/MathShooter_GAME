#include <QApplication>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QPalette>

#include <QTcpSocket>
#include <QNetworkInterface>

#include "PagesWidget.h"

MyBaseStackedWidget::MyBaseStackedWidget(QWidget* parent) : QWidget(parent){
    widgetsList = new QStackedWidget;

    MyFirstPageWidget *firstWidget = new MyFirstPageWidget;
    MySecondPageWidget *secondWidget = new MySecondPageWidget;

    Game *gameWidget = new Game;

    connect(firstWidget, &MyFirstPageWidget::FirstPageFirstBtnButtonPress, this, [this, secondWidget]{widgetsList->setFixedSize(MySecondPageWidget::SIZE); this->widgetsList->setCurrentWidget(secondWidget);});
    connect(secondWidget, &MySecondPageWidget::SecondPageFirstBtnButtonPress,  this, [this, firstWidget]{widgetsList->setFixedSize(MyFirstPageWidget::SIZE); this->widgetsList->setCurrentWidget(firstWidget);});
    connect(secondWidget, &MySecondPageWidget::GamePageSecondBtnButtonPress,  this, [this, gameWidget]{widgetsList->setFixedSize(Game::SIZE); gameWidget->initGame(); this->widgetsList->setCurrentWidget(gameWidget);});

    widgetsList->addWidget(firstWidget);
    widgetsList->addWidget(secondWidget);
    widgetsList->addWidget(gameWidget);

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

    firstBtn = new QPushButton("Create game");

    QVBoxLayout* layoutFirstPage = new QVBoxLayout(this);
    layoutFirstPage->addWidget(firstBtn);

    connect(firstBtn, &QPushButton::clicked, this, [this]{this->emit FirstPageFirstBtnButtonPress();});
}

MyFirstPageWidget::~MyFirstPageWidget()
{
    delete firstBtn;
}

MySecondPageWidget::MySecondPageWidget(QWidget* parent) : QWidget(parent) {
    resize(MySecondPageWidget::SIZE);
    QPalette pal(palette());
    pal.setColor(QPalette::Window, QColorConstants::Svg::pink);
    setAutoFillBackground(true);
    this->setPalette(pal);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    BtnBack = new QPushButton("Back");
    BtnStartGame = new QPushButton("Start game");

    QVBoxLayout* layoutSecondPage = new QVBoxLayout(this);
    layoutSecondPage->addItem(new QSpacerItem(0, 500));
    layoutSecondPage->addWidget(BtnStartGame);
    layoutSecondPage->addWidget(BtnBack);
    layoutSecondPage->addItem(new QSpacerItem(0, 500));

    connect(BtnStartGame, &QPushButton::clicked, this, [this]{this->emit GamePageSecondBtnButtonPress();});
    connect(BtnBack, &QPushButton::clicked, this, [this]{this->emit SecondPageFirstBtnButtonPress();});
}

MySecondPageWidget::~MySecondPageWidget()
{
    delete BtnBack;
    delete BtnStartGame;
}
