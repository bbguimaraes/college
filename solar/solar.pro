QT += opengl webkitwidgets
CONFIG += debug

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH = include
LIBS = -lGLU

OBJECTS_DIR = obj
MOC_DIR = moc

HEADERS = include/*.h
SOURCES = src/*.cpp
