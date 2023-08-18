QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        appointment.cpp \
        database.cpp \
        main.cpp \
        patient.cpp \
        patientHistory.cpp

INCLUDEPATH += \
    -I/../../Database/includes/

#LIBS += \
#    -L/../../Database/lib/ -lsqlite3


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    appointment.h \
    database.h \
    fifo_map.hpp \
    patient.h \
    patientHistory.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Database/lib/ -lsqlite3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Database/lib/ -lsqlite3

INCLUDEPATH += $$PWD/../Database/includes
DEPENDPATH += $$PWD/../Database/includes

DISTFILES += \
    metaFile
