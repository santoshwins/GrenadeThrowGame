QT += opengl
QT += core \
    gui
# going to hard code clang to c++11
#QMAKE_CXX=clang++  -fdiagnostics-fixit-info
DEFINES+=GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
TARGET=bin/Game
DESTDIR=./
OBJECTS_DIR=obj
# this is where we want to put the intermediate build files ( .o)
OBJECTS_DIR=./obj/
MOC_DIR=./moc/
SOURCES += src/main.cpp \
    src/GameModel.cpp \
    src/GameControls.cpp \
    src/GameDrawEngine.cpp \
    src/GameLevel.cpp \
    src/GameModelFactory.cpp \
    src/GamePlayer.cpp \
    src/GameObstacles.cpp \
    src/GamePhysicsEngine.cpp \
    src/GameCrateBox.cpp \
    src/GameLogic.cpp
INCLUDEPATH+=./include
INCLUDEPATH+=/home/anitha/Qtproject/bullet-2.81-rev2613/src/
HEADERS += \
    include/GameModel.h \
    include/GameControls.h \
    include/GameDrawEngine.h \
    include/log.h \
    include/GameLevel.h \
    include/GameModelFactory.h \
    include/GamePlayer.h \
    include/GameObstacles.h \
    include/GamePhysicsEngine.h \
    include/GameCrateBox.h \
    include/GameLogic.h


INCLUDEPATH+=/usr/local/include/bullet
LIBS+= -L/usr/local/lib -lBulletDynamics  -lBulletCollision -lLinearMath


OTHER_FILES+= shaders/Phong.fs \
              shaders/Phong.vs \
    Phong.fs \
    Phong.vs \
    shaders/TextureFragment.glsl \
    shaders/TextureVertex.glsl
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

DEFINES+=GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG


LIBS += -L/usr/lib64/
LIBS += -L/usr/local/lib


# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL

#LIBS+= -L/home/anitha/Qtproject/bullet-2.81-rev2613/src/BulletMultiThreaded\
#LIBS+=  -L/home/anitha/Qtproject/bullet-2.81-rev2613/src/BulletSoftBody\
#  -L/home/anitha/Qtproject/bullet-2.81-rev2613/src/BulletDynamics\
# -L/home/anitha/Qtproject/bullet-2.81-rev2613/src/BulletCollision\
#-L/home/anitha/Qtproject/bullet-2.81-rev2613/src/LinearMath\
#-lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath

#LIBS+= -L/usr/local/lib/ -llibBulletMultiThreaded.la

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-clang {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
linux-g++-64 {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN

# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/

win32: {
        DEFINES+=USING_GLEW
        INCLUDEPATH+=-I c:/boost_1_44_0
        INCLUDEPATH+=-I c:/boost

        INCLUDEPATH+= -I C:/NGL/Support/glew
        LIBS+= -L C:/NGL/lib
        LIBS+= -lmingw32
        DEFINES += WIN32
        DEFINES += USING_GLEW
        DEFINES +=GLEW_STATIC
        DEFINES+=_WIN32
        SOURCES+=C:/NGL/Support/glew/glew.c
        INCLUDEPATH+=C:/NGL/Support/glew/
}

RESOURCES +=

FORMS += \
    ui/GameControls.ui
