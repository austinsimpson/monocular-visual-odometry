QT += core gui widgets 3dextras 3drender

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
        Frame.cpp \
        FrameComparisonWidget.cpp \
        FrameCorrespondence.cpp \
        VideoMapper.cpp \
        TrackedPoint.cpp \
        MainWindow.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

macx: LIBS += -L$$PWD/../../../../usr/local/Cellar/opencv/4.2.0_3/lib/ -lopencv_core.4.2.0 -lopencv_videoio.4.2.0 -lopencv_highgui.4.2.0 -lopencv_features2d.4.2.0 -lopencv_calib3d.4.2.0 -lopencv_imgproc.4.2.0
macx: INCLUDEPATH += $$PWD/../../../../usr/local/Cellar/opencv/4.2.0_3/include/opencv4
macx: DEPENDPATH += $$PWD/../../../../usr/local/Cellar/opencv/4.2.0_3/include/opencv4



HEADERS += \
    Frame.h \
    FrameComparisonWidget.h \
    FrameCorrespondence.h \
    MainWindow.h \
    TrackedPoint.h \
    Utilities.h \
    VideoMapper.h

FORMS += \
    MainWindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../opencv/build/x64/vc15/lib/ -lopencv_world420
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../opencv/build/x64/vc15/lib/ -lopencv_world420d

win32: INCLUDEPATH += C:/opencv/build/include
win32: DEPENDPATH += C:/opencv/build/include
