INCLUDEPATH += $$PWD
HEADERS += $$PWD/zconfserver.h $$PWD/zconftable.h \
	$$PWD/ipctcpserver.h \
	$$PWD/ipctcpsocket.h \
	$$PWD/modelsynchout.h \
	$$PWD/networkinfo.h \
	$$PWD/zconfdef.h
SOURCES += $$PWD/zconfserver.cpp $$PWD/zconftable.cpp \
	$$PWD/ipctcpserver.cpp \
	$$PWD/ipctcpsocket.cpp \
	$$PWD/modelsynchout.cpp \
	$$PWD/networkinfo.cpp \
	$$PWD/zconfdef.cpp
DISTFILES += $$PWD/zconf.model
QT += network
