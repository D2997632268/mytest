
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

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
        main.cpp \
    binaryzation.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH +=D:/MinGW/msys/1.0/work/opencv-3.0.0/build/install/include


CONFIG(debug, debug|release): {
LIBS += -LD:/MinGW/msys/1.0/work/opencv-3.0.0/build/install/x86/mingw/lib/ \
    -lopencv_calib3d300 \
    -lopencv_core300 \
    -lopencv_features2d300 \
    -lopencv_flann300 \
    -lopencv_hal300 \
    -lopencv_highgui300 \
    -lopencv_imgproc300 \
    -lopencv_imgcodecs300 \
    -lopencv_ml300 \
    -lopencv_objdetect300 \
    -llibopencv_photo300 \
    -lopencv_shape300 \
    -lopencv_stitching300 \
    -lopencv_superres300 \
    -lopencv_ts300 \
    -lopencv_video300 \
    -lopencv_videoio300 \
    -lopencv_videostab300





} else:CONFIG(release, debug|release): {
LIBS += -LD:/MinGW/msys/1.0/work/opencv-3.0.0/build/install/x86/mingw/lib/ \
    -lopencv_calib3d300 \
    -lopencv_core300 \
    -lopencv_features2d300 \
    -lopencv_flann300 \
    -lopencv_hal300 \
    -lopencv_highgui300 \
    -lopencv_imgproc300 \
    -lopencv_imgcodecs300 \
    -lopencv_ml300 \
    -lopencv_objdetect300 \
    -llibopencv_photo300 \
    -lopencv_shape300 \
    -lopencv_stitching300 \
    -lopencv_superres300 \
    -lopencv_ts300 \
    -lopencv_video300 \
    -lopencv_videoio300 \
    -lopencv_videostab300



}


QMAKE_CXXFLAGS += -march=i686 -O2

#D:\MinGW\msys\1.0\work\opencv-3.0.0\build\install\x86\mingw\bin
#set PATH=%PATH%;D:\MinGW\msys\1.0\work\opencv-3.0.0\build\install\x86\mingw\bin

HEADERS += \
    binaryzation.h
