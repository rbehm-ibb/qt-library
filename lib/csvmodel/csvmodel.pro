# // ******************************************************
# // * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
# // * All Rights reserved
# // ******************************************************

TEMPLATE = app
include( $$[PRTEMPLATE]/project.pri )
include( $$[PRTEMPLATE]/lib/utils/stdicons.pri )
include( $$[PRTEMPLATE]/lib/utils/toolbarspacer.pri )
include( $$[PRTEMPLATE]/lib/csvmodel/csvmodel.pri )

QT += core gui widgets

HEADERS += \
    mainwindow.h

SOURCES +=  \
    main.cpp \
    mainwindow.cpp
