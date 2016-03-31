TEMPLATE = app
CONFIG *= testcase c++11
QT *= testlib

INCLUDEPATH = ../lib/include
POST_TARGETDEPS = ../lib/libtransactions.a
SOURCES = main.cpp
LIBS = ../lib/libtransactions.a
