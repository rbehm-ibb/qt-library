INCLUDEPATH += $$PWD
HEADERS += $$PWD/zconfclient.h $$PWD/zconftable.h \
	$$PWD/ipctcpsocket.h \
	$$PWD/modelsynchin.h \
#	$$PWD/modelsynchout.h \
	$$PWD/networkinfo.h \
	$$PWD/zconfdef.h
SOURCES += $$PWD/zconfclient.cpp $$PWD/zconftable.cpp \
	$$PWD/ipctcpsocket.cpp \
	$$PWD/modelsynchin.cpp \
	$$PWD/networkinfo.cpp \
	$$PWD/zconfdef.cpp
DISTFILES += $$PWD/zconf.model
QT += network
