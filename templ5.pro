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
include( $$[PRTEMPLATE]/lib/utils/filenamehandler.pri )
include( $$[PRTEMPLATE]/lib/utils/ibtoolbar.pri )
include( $$[PRTEMPLATE]/lib/utils/ledicon/ledicon.pri )
include( $$[PRTEMPLATE]/lib/utils/filetextstream/filetextstream.pri )
include( $$[PRTEMPLATE]/lib/serialport/ibserialport.pri )
include( $$[PRTEMPLATE]/lib/osmview/osmwidget.pri )
include( $$[PRTEMPLATE]/lib/utils/commsocket/commserver.pri )

HEADERS += \
 
SOURCES +=  \

RESOURCES += \
    rsc.qrc

DISTFILES += \
    defaults.rc \
    styles.css
