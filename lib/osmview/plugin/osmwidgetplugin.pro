include( $$[PRTEMPLATE]/project.pri )
# include( $$[PRTEMPLATE]/lib/osmview/osmwidget.pri )

#! [0]
QT          += gui widgets network designer
#QT          += uiplugin
#! [0]

QTDIR_build {
# This is only for the Qt build. Do not use externally. We mean it.
PLUGIN_TYPE = designer

PLUGIN_CLASS_NAME = OsmWidgetPlugin
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

INCLUDEPATH += ../../../../templateQt5/lib/osmview

#! [3]
HEADERS     =  \
	      osmwidgetplugin.h \
#    osmview.h \
#    osmtile.h \
#    osmscene.h \
#    osmwidget.h \
#    mapscalewidget.h
    ../../../../templateQt5/lib/osmview/osmscene.h \
    ../../../../templateQt5/lib/osmview/osmtile.h \
    ../../../../templateQt5/lib/osmview/osmview.h \
    ../../../../templateQt5/lib/osmview/osmwidget.h

SOURCES     =  \
	      osmwidgetplugin.cpp \
#    osmview.cpp \
#    osmtile.cpp \
#    osmscene.cpp \
#    osmwidget.cpp \
#    mapscalewidget.cpp
    ../../../../templateQt5/lib/osmview/osmscene.cpp \
    ../../../../templateQt5/lib/osmview/osmtile.cpp \
    ../../../../templateQt5/lib/osmview/osmview.cpp \
    ../../../../templateQt5/lib/osmview/osmwidget.cpp

RESOURCES += \
    plugin_icon.qrc

OTHER_FILES += osmwidget.json
#! [3]

FORMS += \
