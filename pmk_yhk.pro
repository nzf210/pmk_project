
QT      += core gui sql
QT      += webenginewidgets
QT      +=printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#LIBS += -lqtfreetype
SOURCES += \
    form.cpp \
    main.cpp \
    mainwindow.cpp \
    pdf_dok.cpp

HEADERS += \
    form.h \
    mainwindow.h \
    pdf.h \
    pdf2.h \
    pdf3.h \
    pdf4.h \
    pdf_dok.h \
    qrencode.h

FORMS += \
    form.ui \
    mainwindow.ui \
    pdf_dok.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#LIBS += -lqrencode

#INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\um"
#LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\um\x64"

#INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt"
#LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\ucrt\x64"

INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\um"
LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\um\x86"

INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt"
LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\ucrt\x86"

RESOURCES += \
    src.qrc

#ICON = /gbr/html/gbr/yhk.png
ICON = /gbr/html/gbr/yhk.ico
#RC_ICONS = yhk.ico
#win32:
#ICONS = myIcon.ico

