# copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
# All Rights reserved

QT += gui widgets network

INCLUDEPATH += . $$PWD

SOURCES += \
	$$PWD/mapscalewidget.cpp \
    $$PWD/osmview.cpp \
    $$PWD/osmtile.cpp \
    $$PWD/osmscene.cpp \
    $$PWD/osmwidget.cpp \
#    $$PWD/mapscalewidget.cpp

HEADERS += \
	$$PWD/mapscalewidget.h \
    $$PWD/osmview.h \
    $$PWD/osmtile.h \
    $$PWD/osmscene.h \
    $$PWD/osmwidget.h \
#    $$PWD/mapscalewidget.h

RESOURCES += \
    $$PWD/osm_icons.qrc

DEFINES += HAS_OSMWIDGET
