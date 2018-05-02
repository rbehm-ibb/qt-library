INCLUDEPATH += $$PWD
HEADERS += \
    $$PWD/ibserialport.h \
    $$PWD/ibserialportline.h \
    $$PWD/ibserialportnmea.h
SOURCES += \
    $$PWD/ibserialport.cpp \
    $$PWD/ibserialportline.cpp \
    $$PWD/ibserialportnmea.cpp \
    $$PWD/ibserialportnmea-send.cpp \
    $$PWD/ibserialportnmea-decode.cpp

QT += serialport
