TEMPLATE = lib
CONFIG *= staticlib c++11
QT =
TARGET = transactions

DEPENDPATH = include
INCLUDEPATH = $${DEPENDPATH}

HEADERS = include/*.h
SOURCES = src/*.cpp

OBJECTS_DIR = obj
