TEPLATE = app
CONFIG += qt c++11
QT += opengl

OBJECTS_DIR = obj
MOC_DIR = moc

INCLUDEPATH += include/
HEADERS += include/gui/*.h include/model/*.h include/net/*.h
SOURCES += src/main.cpp src/gui/*.cpp src/model/*.cpp src/net/*.cpp
LIBS += -lGLU

QMAKE_CLEAN += test/single/single
