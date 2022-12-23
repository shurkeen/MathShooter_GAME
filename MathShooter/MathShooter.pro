QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    field.cpp \
    game.cpp \
    graph.cpp \
    main.cpp \
    mathcalculator.cpp \
    mathchecker.cpp \
    mathexpression.cpp \
    mathparser.cpp \
    obstacle.cpp \
    player.cpp

HEADERS += \
    field.h \
    game.h \
    graph.h \
    mathcalculator.h \
    mathchecker.h \
    mathexpression.h \
    mathparser.h \
    obstacle.h \
    player.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
