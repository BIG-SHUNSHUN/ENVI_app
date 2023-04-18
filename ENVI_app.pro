QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Function.cpp \
    src/Image.cpp \
    src/ImageFactory.cpp \
    src/ImageManager.cpp \
    src/ImageView.cpp \
    src/ImageViewManager.cpp \
    src/ROI.cpp \
    src/TreeViewWrapper.cpp \
    src/displaypanel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    src/headers/Function.h \
    src/headers/Image.h \
    src/headers/ImageFactory.h \
    src/headers/ImageManager.h \
    src/headers/ImageView.h \
    src/headers/ImageViewManager.h \
    src/headers/ROI.h \
    src/headers/TreeViewWrapper.h \
    src/headers/displaypanel.h \
    mainwindow.h

FORMS += \
    src/ui/displaypanel.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    MainWindow.qrc

win32: LIBS += -LC:/dev_tools/opencv460-vs2019-debug/x64/vc16/lib/ -lopencv_world460d

INCLUDEPATH += /usr/local/include/opencv4
DEPENDPATH += /usr/local/include/opencv4

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgcodecs
