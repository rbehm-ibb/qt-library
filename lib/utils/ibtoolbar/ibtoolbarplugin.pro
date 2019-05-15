include( $$[PRTEMPLATE]/project.pri )
# include( $$[PRTEMPLATE]/lib/osmview/osmwidget.pri )

#! [0]
QT          += gui widgets network designer
#QT          += uiplugin
#! [0]

QTDIR_build {
# This is only for the Qt build. Do not use externally. We mean it.
PLUGIN_TYPE = designer

PLUGIN_CLASS_NAME = IbToolBarPlugin
load(qt_plugin)
} else {
# Public example:

#! [2]
CONFIG      += plugin
TEMPLATE    = lib
#! [2]

TARGET = $$qtLibraryTarget($$TARGET)

target.path = /home/behm/Qtc34/Tools/QtCreator/bin/plugins/designer
INSTALLS += target

}

INCLUDEPATH += ../../../../templateQt5/lib/utils

#! [3]
HEADERS     =  \
	      ibtoolbarplugin.h \
    ../ibtoolbar.h

SOURCES     =  \
	      ibtoolbarplugin.cpp \
    ../ibtoolbar.cpp

RESOURCES += \
    ibtoolbar.qrc \
    ibtoolbar_plugin.qrc


OTHER_FILES += osmwidget.json
#! [3]

FORMS += \
