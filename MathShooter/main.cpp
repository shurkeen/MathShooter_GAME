#include <QApplication>
#include "game.h"
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game* m_game = new Game;
    m_game->initGame();
    m_game->show();

    return a.exec();
}
