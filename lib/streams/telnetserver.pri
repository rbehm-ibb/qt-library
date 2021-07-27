INCLUDEPATH += $$PWD
HEADERS += $$PWD/telnetserver.h $$PWD/telnetsocket.h \
	$$PWD/streamlinefilter.h
SOURCES += $$PWD/telnetserver.cpp $$PWD/telnetsocket.cpp \
	$$PWD/streamlinefilter.cpp
QT += network
