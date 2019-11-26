QT_CONFIG -= no-pkg-config
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
#CONFIG -= qt
QMAKE_LFLAGS += -Wall -fPIC -no-pie

DEFINES += DEBUG

SOURCES += \
    Ports.c \
    files.c \
    main.c

HEADERS += \
    CLReader_api.h \
    Ports.h \
    files.h \
    main.h

INCLUDEPATH += $$PWD/
        $$PWD/.

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/./ -lapi
INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
unix:!macx: PRE_TARGETDEPS += $$PWD/./libapi.a

DISTFILES +=
