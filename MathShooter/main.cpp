#include <QApplication>
#include <QHBoxLayout>
#include "client.h"
#include "game.h"
#include "server.h"
#include "pageswidget.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItemGroup>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // Server server;
//    Game* m_game = new Game;
//    m_game->initGame();
//    m_game->show();

    MyBaseStackedWidget baseWidget;

    baseWidget.MyShow();

//    QMainWindow *win1 = new QMainWindow();
//    win1->resize(800, 800);
//    win1->show();

//    QGraphicsScene *scene = new QGraphicsScene(win1);
//    QGraphicsView vieww(scene, win1);
//    vieww.show();
//    vieww.resize(600, 600);

//    QPixmap pix(300, 300);
//    QPixmap pix2(500, 500);
//    pix.fill(Qt::transparent);
//    QPainter *pain2 = new QPainter(&pix2);
//    QPainter *pain = new QPainter(&pix);

//    pain->setPen(QColor(255, 34, 255, 255));
//    pain->drawRect(15, 15, 100, 100);

//    pain2->setPen(QColor(255, 34, 255, 255));
//    pain2->drawRect(30, 30, 400, 400);

//    delete pain;
//    delete pain2;
//    scene->addPixmap(pix2);
//    scene->addPixmap(pix);

//    QGraphicsItemGroup *item1 = new QGraphicsItemGroup();
//    QGraphicsItemGroup *item2 = new QGraphicsItemGroup();

//    scene->addItem(item1);
//    scene->addItem(item2);

//    item1->addToGroup(scene->addLine(100, 100, 200, 200));
//    item2->addToGroup(scene->addLine(100, 200, 200, 200));

//    Client* client = new Client;
//    client->show();

    return a.exec();
}
