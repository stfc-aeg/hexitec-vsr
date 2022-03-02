#-------------------------------------------------
#
# Project created by QtCreator 2011-11-24T10:35:18
#
#-------------------------------------------------
QT       += core gui script serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Prevent MSVC 2013 complaining "Conversion from string literal loses const qualifier"
QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
# See: http://stackoverflow.com/a/28625430/2903608

TARGET = HexitecVSR
TEMPLATE = app

SOURCES += main.cpp \
    voxel.cpp \
    thumbviewer.cpp \
    slice.cpp \
#    scriptingwidget.cpp \
    sarray.cpp \
    renderarea.cpp \
    plotter.cpp \
    plotsettings.cpp \
#    motorlimits.cpp \
#    motor.cpp \
    mainwindow.cpp \
    hxtframe.cpp \
    hxtfilereader.cpp \
    gridsizequery.cpp \
#    dummymotor.cpp \
#    motioncontrolform.cpp \
    curve.cpp \
    colormap.cpp \
    chargesharing.cpp \
    axissettings.cpp \
    treeitem.cpp \
    colormapeditor.cpp \
#    motormodel.cpp \
#    motordelegate.cpp \
    dataacquisitionfactory.cpp \
#    motorfactory.cpp \
#    galilmotorcontroller.cpp \
#    galilmotor.cpp \
#    matlab.cpp \
    applicationoutput.cpp \
    hexitecsofttrigger.cpp \
    reservable.cpp \
#    scriptrunner.cpp \
    objectreserver.cpp \
    reservation.cpp \
#    motorstatus.cpp \
    volume.cpp \
    datamodel.cpp \
    mainviewer.cpp \
    workspace.cpp \
    getvaluedialog.cpp \
#    matlabvariable.cpp \
    eigencomponent.cpp \
    offsetsdialog.cpp \
    datacollectiondialog.cpp \
    dpwarningdialog.cpp \
    vboutofrangedialog.cpp \
    detectorcontrolform.cpp \
    dataacquisitionform.cpp \
    dataacquisition.cpp \
    voltageSourceFactory.cpp \
    dataacquisitionmodel.cpp \
    detectorfilename.cpp \
    dataacquisitiondefinition.cpp \
    dataacquisitionstatus.cpp \
#    newportxpsmotor.cpp \
    setaxesdialog.cpp \
    displaylegendsdialog.cpp \
    parameters.cpp \
    hv.cpp \
    badinifiledialog.cpp \  
    progressform.cpp \
    pixelmanipulationform.cpp \
    processingbuffergenerator.cpp \
    processingForm.cpp \
    imageprocessorhandler.cpp

HEADERS += mainwindow.h \
    voxel.h \
    slice.h \
#    scriptingwidget.h \
    sarray.h \
    renderarea.h \
    plotter.h \
    plotsettings.h \
#    motorlimits.h \
#    motor.h \
    hxtframe.h \
    hxtfilereader.h \
    gridsizequery.h \
#    dummymotor.h \
#    motioncontrolform.h \
    curve.h \
    colormap.h \
    chargesharing.h \
    axissettings.h \
    thumbviewer.h \
    treeitem.h \
    colormapeditor.h \
#    motormodel.h \
#    motordelegate.h \
    dataacquisitionfactory.h \
#    motorfactory.h \
#    galilmotorcontroller.h \
#    galilmotor.h \
#    matlab.h \
    applicationoutput.h \
#    hardtrigger.h \
    hexitecsofttrigger.h \
    reservable.h \
#    scriptrunner.h \
    objectreserver.h \
    reservation.h \
#    motorstatus.h \
    volume.h \
    datamodel.h \
    mainviewer.h \
    workspace.h \
    getvaluedialog.h \
#    matlabvariable.h \
    eigencomponent.h \
    offsetsdialog.h \
    datacollectiondialog.h \
    dpwarningdialog.h \
    vboutofrangedialog.h \
    detectorcontrolform.h \
    dataacquisitionform.h \
    dataacquisition.h \
    voltageSourceFactory.h \
    dataacquisitionmodel.h \
    detectorfilename.h \
    dataacquisitiondefinition.h \
    dataacquisitionstatus.h \
#    newportxpsmotor.h \
    setaxesdialog.h \
    displaylegendsdialog.h \
    parameters.h \
    hv.h \
    badinifiledialog.h \
    progressform.h \
    pixelmanipulationform.h \
    processingbuffergenerator.h \
    processingForm.h \
    imageprocessorhandler.h

FORMS += \
    gridsizequery.ui \
    chargesharing.ui \
#    scriptingWidget.ui \
    colormapeditor.ui \
    getvaluedialog.ui \
    offsetsdialog.ui \
    datacollectiondialog.ui \
    dpwarningdialog.ui \
    vboutofrangedialog.ui \
#    motioncontrolform.ui \
    detectorcontrolform.ui \
    dataacquisitionform.ui \
    setaxesdialog.ui \
    displaylegendsdialog.ui \
    badinifiledialog.ui \
    progressform.ui \
    pixelmanipulationform.ui \
    processingForm.ui

RESOURCES += \
    HexitecVSR.qrc

# The HexitecVSR_SYSTEM environment variable determines which configuration is performed next.
#CONFIG += $$(HEXITECVSR_SYSTEM)
CONFIG += $$(HEXITECGIGE_SYSTEM)

shk78_devel {
    # te2Scandinavia/te2fuscot PC 64 bit configuration
    DEFINES += NONI
    DEFINES += OS_WIN
    LIBS += delayimp.lib
    QMAKE_LFLAGS_RELEASE += /DELAYLOAD:VSR.dll #/DELAYLOAD:libeng.dll /DELAYLOAD:libmx.dll
    LIBS += ../VSRLib/VSR.lib
    LIBS += $$quote(-LC:/Program Files (x86)/Pleora Technologies Inc/eBUS SDK/Libraries)
    CONFIG(release, debug|release): LIBS += ../VSRLib/release/VSRLib.lib ../SerialPortLib/release/SerialPortLib.lib
    CONFIG(release, debug|release): LIBS += ../HxtProcessingLib/release/HxtProcessingLib.lib
    CONFIG(debug, debug|release): LIBS += ../VSRLib/debug/VSRLib.lib ../SerialPortLib/debug/SerialPortLib.lib
    CONFIG(debug, debug|release): LIBS += ../HxtProcessingLib/debug/HxtProcessingLib.lib
    INCLUDEPATH += "C:/Program Files (x86)/Pleora Technologies Inc/eBUS SDK/Includes"
    INCLUDEPATH += "C:/Qt/Libs/Eigen/3.3.4/Eigen/"
    INCLUDEPATH += ../VSRLib
    INCLUDEPATH  += ../HxtProcessingLib/include
    INCLUDEPATH  += ../HxtProcessingLib/
    INCLUDEPATH += ../SerialPortLib
} else:ckd_devel {
    DEFINES += NONI
    DEFINES += OS_WIN
    LIBS += delayimp.lib
    QMAKE_LFLAGS_RELEASE += /DELAYLOAD:VSR.dll #/DELAYLOAD:libeng.dll /DELAYLOAD:libmx.dll
    LIBS += ../VSRLib/VSR.lib
    LIBS += $$quote(-LC:/Program Files (x86)/Pleora Technologies Inc/eBUS SDK/Libraries)
    CONFIG(release, debug|release): LIBS += ../VSRLib/release/VSRLib.lib ../SerialPortLib/release/SerialPortLib.lib
    CONFIG(release, debug|release): LIBS += ../HxtProcessingLib/release/HxtProcessingLib.lib
    CONFIG(debug, debug|release): LIBS += ../VSRLib/debug/VSRLib.lib ../SerialPortLib/debug/SerialPortLib.lib
    CONFIG(debug, debug|release): LIBS += ../HxtProcessingLib/debug/HxtProcessingLib.lib
    INCLUDEPATH += "C:/Program Files (x86)/Pleora Technologies Inc/eBUS SDK/Includes"
    INCLUDEPATH += "C:/Qt/Libs/Eigen/3.3.7/Eigen/"
    INCLUDEPATH += ../VSRLib
    INCLUDEPATH  += ../HxtProcessingLib/include
    INCLUDEPATH  += ../HxtProcessingLib/
    INCLUDEPATH += ../SerialPortLib
}

