#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include "field.h"

class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QWidget* parent = 0);
    void initGame();
    void gettingCoord();

signals:
    void coordUpdated();

public slots:
    void testt(QVector<int> zzz);

private:
    Field* m_field;
    QPushButton* fireBtn;
    QPushButton* quitBtn;
    QLineEdit* functionInput;
    QLabel* historyInput;
    QVector<QPoint> m_dots;
};

#endif // GAME_H
