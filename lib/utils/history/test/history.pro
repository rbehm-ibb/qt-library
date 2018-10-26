# // ******************************************************
# // * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
# // * All Rights reserved
# // ******************************************************

TEMPLATE = app
include( $$[PRTEMPLATE]/project.pri )
include( $$[PRTEMPLATE]/lib/utils/stdicons.pri )
include( $$[PRTEMPLATE]/lib/utils/ibb-logo.pri )
include( $$[PRTEMPLATE]/lib/utils/config.pri )
include( $$[PRTEMPLATE]/lib/utils/singleapp/singleapp.pri )
include( $$[PRTEMPLATE]/lib/utils/history/history.pri )
include( $$[PRTEMPLATE]/lib/utils/ibtoolbar.pri )

HEADERS += \
    mainwindow.h

SOURCES +=  \
    main.cpp \
    mainwindow.cpp

RESOURCES += \
    rsc/rsc.qrc

DISTFILES += \
    rsc/defaults.rc \
    rsc/styles.css \
    rsc/list.png

FORMS += \
    mainwindow.ui

