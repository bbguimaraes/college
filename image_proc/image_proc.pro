TEPLATE = app
CONFIG += qt
QT += opengl

QMAKE_CXXFLAGS = -std=c++11

OBJECTS_DIR = obj
MOC_DIR = moc

HEADERS += include/*.h
SOURCES += src/*.cpp
