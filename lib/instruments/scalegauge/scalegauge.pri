INCLUDEPATH += $$PWD
HEADERS +=  \
    $$PWD/abstractscale.h \
    $$PWD/barscale.h \
    $$PWD/arcscale.h

SOURCES +=  \
    $$PWD/abstractscale.cpp \
    $$PWD/barscale.cpp \
    $$PWD/arcscale.cpp

#RESOURCES += $$PWD/scalegauge.qrc
#DISTFILES += $$PWD/scalegauge.css

QT += core gui widgets

