TEMPLATE = app
#CONFIG += console
#CONFIG -= qt
CONFIG += console
QMAKE_CFLAGS_RELEASE +=-std=c99
QMAKE_CXXFLAGS_DEBUG+=-std=c99
SOURCES += main.c \
    binary.c

HEADERS += \
    test_include.h

OTHER_FILES += \
    Makefile \
    Makefile.Release

