######################################################################
# Automatically /usr/local/lib/libmp3lame.a  to mp3 generated by qmake (2.01a) Fr. Okt 25 09:56:53 2013
######################################################################
!include( ../config.pri ) {
message( "../config.pri not found" )
## here inlude lib rtf odt epub format documents
}



qtHaveModule(svg) {
QT += svg
message( "load svg module... for images" )
} else {
DEFINES += _NOSVGMODULE_
message( "svg module disable ... for images svg" )
}

qtHaveModule(pdfium) {
QT += pdfium
message( "load pdfium module... for pdt text extract" )
} else {
DEFINES += _NO_PDFIUM_MODULE_
message( "pdfium module disable not found... " )


qtHaveModule(notexitmodule) {
POPPLERDIR = /usr/local/include/poppler/qt5
##### libpoppler-qt5.1.3.0.dylib
##### /usr/local/lib/libpoppler-qt5.1.3.0.dylib
DEPENDPATH +=  $$POPPLERDIR
INCLUDEPATH += $$POPPLERDIR
LIBS +=/usr/local/lib/libpoppler-qt5.1.3.0.dylib
DEFINES += _QT5POPPLEROK_
message( " Load local poppler static lib /POPPLERPDFQT4 load static" )
}
}

qtHaveModule(core) {
#############module xxdebug?###############release!##########################################
TEMPLATE = app
TARGET = ../OasisEdit
CONFIG +=release warn_off silence
RC_FILE = images/ODTicon.icns
QMAKE_INFO_PLIST = Info.plist
} else {
############################debug!##########################################
TEMPLATE = app
TARGET = sax
CONFIG-=app_bundle
CONFIG +=debug_release warn_on console
}


DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += imageedit.h \
    oasimain.h \
    editorkernel.h \
    editortable_setting.h \
    ui_forms.h \
    doc_session.h \
    oasi_application.h \
    editvoiceblock.h \
    voiceprocesing.h \
    worker.h

SOURCES += main.cpp \
    imageedit.cpp \
    oasimain.cpp \
    editorkernel.cpp \
    editortable_setting.cpp \
    doc_session.cpp \
    oasi_application.cpp \
    editvoiceblock.cpp \
    voiceprocesing.cpp \
    worker.cpp

RESOURCES += textedit.qrc

DISTFILES += copy_session











