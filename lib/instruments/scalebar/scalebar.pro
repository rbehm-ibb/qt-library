# // ******************************************************
# // * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
# // * All Rights reserved
# // ******************************************************

TEMPLATE = app
include( $$[PRTEMPLATE]/project.pri )
include( $$[PRTEMPLATE]/lib/utils/stdicons.pri )
include( $$[PRTEMPLATE]/lib/instruments/scalebar/scalebar.pri )

HEADERS += \
    mainwindow.h

SOURCES +=  \
    main.cpp \
    mainwindow.cpp
