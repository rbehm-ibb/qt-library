DEFINES += QCUSTOMPLOT_USE_LIBRARY QCUSTOM_PLOT
LIBS += -L/usr/local/qcustomplot/libqt5/ -lqcustomplot
INCLUDEPATH += /usr/local/qcustomplot/include

# needed for this lib
QT += core gui widgets printsupport
