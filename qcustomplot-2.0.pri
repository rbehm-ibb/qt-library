DEFINES += QCUSTOMPLOT_USE_LIBRARY QCUSTOM_PLOT QCUSTOMPLOT_USE_OPENGL
LIBS += -L/usr/local/qcustomplot-2.0/libqt5/ -lqcustomplot
INCLUDEPATH += /usr/local/qcustomplot-2.0/include

# needed for this lib
QT += core gui widgets printsupport
