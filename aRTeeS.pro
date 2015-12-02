#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T18:50:56
#
#-------------------------------------------------

QT       += core gui
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aRTeeS
TEMPLATE = app
CONFIG += c++14
LIBS += `pkg-config opencv --libs`

SOURCES += main.cpp\
    MatToQImage.cpp \
    MainWindow.cpp \
    patternidtemplate.cpp



HEADERS  += \
    MatToQImage.h \
    MainWindow.h \
    ImageCapture.h \
    CommonInclude.h \
    ForegroundFilter.h \
    Prepro.h \
    satlib.h \
    TemplateClassifier.h \
    DataExtraction.h \
    RegionProccessing.h \
    Reconstruction.h

FORMS    += mainwindow.ui
