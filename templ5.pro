# // ******************************************************
# // * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
# // * All Rights reserved
# // ******************************************************

TEMPLATE = app
include( $$[PRTEMPLATE]/project.pri )
include( $$[PRTEMPLATE]/lib/utils/stdicons.pri )
include( $$[PRTEMPLATE]/lib/utils/ibb-logo.pri )
include( $$[PRTEMPLATE]/lib/utils/config.pri )
include( $$[PRTEMPLATE]/lib/utils/filenamehandler.pri )
include( $$[PRTEMPLATE]/lib/utils/toolbarspacer.pri )
include( $$[PRTEMPLATE]/lib/utils/ledicon/ledicon.pri )
include( $$[PRTEMPLATE]/lib/utils/filetextstream/filetextstream.pri )
include( $$[PRTEMPLATE]/lib/serialport/ibserialport.pri )

QT += core gui widgets

HEADERS += \
 
SOURCES +=  \
