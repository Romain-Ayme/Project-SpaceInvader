TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        nsutil.cpp

DISTFILES += \
    Invader.img \
    Invader.txt \
    SpaceInvadertext.txt \
    settings.yml

HEADERS += \
    nsutil.h
