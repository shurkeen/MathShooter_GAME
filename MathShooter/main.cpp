#include <QApplication>
#include <QHBoxLayout>
#include "client.h"
#include "game.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server server;
//    Game* m_game = new Game;
//    m_game->initGame();
//    m_game->show();

    Client* client = new Client;
    client->show();

    return a.exec();
}
