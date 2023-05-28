#ifndef PAGESWIDGET_H
#define PAGESWIDGET_H

#include "game.h"
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>

class MyBaseStackedWidget : public QWidget{
    Q_OBJECT

public:
    MyBaseStackedWidget(QWidget* parent = nullptr);
    void MyShow();
    QStackedWidget* widgetsList;
    ~MyBaseStackedWidget();
};

class MyFirstPageWidget : public QWidget{
    Q_OBJECT

public:
    MyFirstPageWidget(QWidget* parent = nullptr);
    QPushButton *firstBtn;
    ~MyFirstPageWidget();
    constexpr static const QSize SIZE = QSize(500, 500);

signals:
    void FirstPageFirstBtnButtonPress();
};

class MySecondPageWidget : public QWidget{
    Q_OBJECT

public:
    MySecondPageWidget(QWidget* parent = 0);
    QPushButton *BtnBack;
    QPushButton *BtnStartGame;
    ~MySecondPageWidget();
    constexpr static const QSize SIZE = QSize(600, 600);

signals:
    void SecondPageFirstBtnButtonPress();
    void GamePageSecondBtnButtonPress();
};

#endif // PAGESWIDGET_H
