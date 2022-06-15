
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
    page_add.cpp \
    page_bamuskam.cpp \
    page_blt.cpp \
    page_covid.cpp \
    page_dds.cpp \
    page_gab.cpp \
    page_gab_add.cpp \
    page_sppd.cpp \
    page_tambahan.cpp \
    page_user.cpp \
    pdf_dok.cpp

HEADERS += \
    form.h \
    mainwindow.h \
    page_user.h \
    pdf_add.h \
    pdf_blt.h \
    pdf_covid.h \
    pdf_dds.h \
    pdf_dok.h \
    pdf_sppd_add.h \
    pdf_sppd_add_gab.h \
    pdf_sppd_all_cair.h \
    pdf_sppd_dds.h \
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

#INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\um"
#LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\um\x86"

#INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt"
#LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\ucrt\x86"

INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um"
LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x86"

INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt"
LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\ucrt\x86"

RESOURCES += \
    src.qrc

RC_ICONS = D:\002_Syahril_Data\Github\syahril\pmk_yhk_21_add\html\gbr\yhk.ico


