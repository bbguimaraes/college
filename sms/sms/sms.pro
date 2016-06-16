TEMPLATE = app
CONFIG *= c++11
QT += opengl

OBJECTS_DIR = obj
MOC_DIR = moc

INCLUDEPATH += ../libsms/include include
DEPENDPATH += include ../lib/include
LIBS += -L../libsms -lsms -lGLU

HEADERS = include/*.h
SOURCES = src/*.cpp
