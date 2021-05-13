#-------------------------------------------------
#
# Project created by QtCreator 2021-05-10T10:50:21
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = collect_sensor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Timer/Timer.cpp \
    magsensor.cpp \
    sensorcontrol.cpp \
    analogbutter.cpp \
    thermocouple.cpp \
    Sensoray/Sensoray.cpp \
    electromagnet-calibration/scalorPotential.cpp \
    electromagnet-calibration/electromagnet_calibration.cpp \

HEADERS  += mainwindow.h \
    Timer/Timer.h \
    ../Libraries/PolarisSpectra/PolarisSpectra.h \
    polaristransformmatrix.h \
    sensorcontrol.h \
    magsensor.h \
    analogbutter.h \
    thermocouple.h \
    Sensoray826/826api.h \
    Sensoray/Sensoray.h \
    electromagnet-calibration/scalorPotential.h \
    electromagnet-calibration/electromagnet_calibration.h \
    electromagnet-calibration/EigenToYAML.h

FORMS    += mainwindow.ui


unix:!macx: LIBS += -L$$PWD/../Libraries/PolarisSpectra/ -lPolarisSpectraLinux -lrt -L/home/telerobotics/src/collect_sensor/Sensoray826 -l826_64 -lm


INCLUDEPATH += $$PWD/../Libraries/PolarisSpectra
DEPENDPATH += $$PWD/../Libraries/PolarisSpectra

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Libraries/yaml-cpp-folder/release/ -lyaml-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Libraries/yaml-cpp-folder/debug/ -lyaml-cpp
else:unix: LIBS += -L$$PWD/../Libraries/yaml-cpp-folder/ -lyaml-cpp

INCLUDEPATH += $$PWD/../Libraries/yaml-cpp-folder/include
DEPENDPATH += $$PWD/../Libraries/yaml-cpp-folder/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Libraries/yaml-cpp-folder/release/libyaml-cpp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Libraries/yaml-cpp-folder/debug/libyaml-cpp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Libraries/yaml-cpp-folder/release/yaml-cpp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Libraries/yaml-cpp-folder/debug/yaml-cpp.lib
else:unix: PRE_TARGETDEPS += $$PWD/../Libraries/yaml-cpp-folder/libyaml-cpp.a

DISTFILES += \
    PolarisSpectraSettings.ini \
    PolarisSpectraErrors.ini \
    initial_guess.yaml

