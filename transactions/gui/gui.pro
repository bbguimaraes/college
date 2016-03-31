TEMPLATE = app
CONFIG *= qt c++11
QT += widgets

INCLUDEPATH = include ../lib/include
DEPENDPATH = $$INCLUDEPATH ../lib

HEADERS = include/*.h
SOURCES = src/*.cpp

LIBS = ../lib/libtransactions.a

OBJECTS_DIR = obj
MOC_DIR = moc
