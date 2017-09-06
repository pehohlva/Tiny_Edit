greaterThan(QT_MAJOR_VERSION, 4) {
DEFINES += _QT5RUNNING_
message(Hi ... Hello we build now or not... on QT5 -Author)
message(We use QT from: $$[QT_INSTALL_PREFIX])
}
########

LIBS +=$$PWD/liboasidoc.a
INCLUDEPATH += $$PWD/src_lib/docformat/rtf-qt $$PWD/src_lib/docformat $$PWD//src_lib/docformat/ooo $$PWD/src_lib/docformat/epub
DEPENDPATH += $$PWD/src_lib/docformat/rtf-qt $$PWD/src_lib/docformat $$PWD//src_lib/docformat/ooo $$PWD/src_lib/docformat/epub

MOC_DIR = o
RCC_DIR = o
OBJECTS_DIR = o

equals(QT_VER_MAJ,4) {
DEFINES += _QT4RUNNING_
message(Hi ... Hello we build now or not... on QT4 if i possibel?? intall QT5 is not run.. )
message(My compiler an not build QT4 :-( -Author ))
message(We use QT from: $$[QT_INSTALL_PREFIX])
}

greaterThan(QT_MAJOR_VERSION, 4) {
DEFINES += _QT5RUNNING_
QT += widgets
}

qtHaveModule(gui) {
QT += gui
message("Oh you have intall printsupport, confirm/bundle.")
}

qtHaveModule(printsupport) {
QT += printsupport
DEFINES += _PRINTERIOK_
message("Oh you have intall printsupport, confirm/bundle.")
} else {
DEFINES += _NOPRINT_
DEFINES += QT_NO_PRINTER
message("Oh you NOT! intall printsupport, confirm QT_NO_PRINTER confirm.")
}
qtHaveModule(xml) {
DEFINES += _HAVEXMLPARSE_
QT += xml
message("Oh you have intall xml, confirm/bundle.")
}
qtHaveModule(network) {
QT += network
message("Oh you have intall network, confirm/bundle.")
}

qtHaveModule(concurrent) {
QT += concurrent
message("Oh you have intall concurrent, confirm/bundle.")
}

### better quality on mac shell say  (man say) in osx console
qtHaveModule(nooootexttospeech) {
DEFINES += _HAVINGNESONSPEECH_
QT += texttospeech
message("Oh you have intall texttospeech, ok we use it..lol.. confirm/bundle ")
}

qtHaveModule(xxxxcl2) {
INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
LIBS +=/usr/local/lib/libqcld2.a
}


######################################################################
#### I not like if QTCreator continue to build if i not like cpu high
#### in this case i write (qtHaveModule notexit)
#########here core or notexistmodule #################################
qtHaveModule(core) {
QT += core
message("Oh you have intall core, confirm/bundle.")
message( "PWD: $$PWD --")
message( "QT_VERSION: $$[QT_VERSION]--")
message( "QMAKE_MKSPECS: $$[QMAKE_MKSPECS]--")
message( "QT_INSTALL_LIBS: $$[QT_INSTALL_LIBS]--")
message( "QT_INSTALL_CONFIGURATION: $$[QT_INSTALL_CONFIGURATION]--")
message( "QT_INSTALL_DATA: $$[QT_INSTALL_DATA]--")
message( "QT_INSTALL_DOCS: $$[QT_INSTALL_DOCS]--")
message( "QT_INSTALL_PLUGINS: $$[QT_INSTALL_PLUGINS]--")
message( "QT_INSTALL_PREFIX: $$[QT_INSTALL_PREFIX]--")
message( "QT_INSTALL_TRANSLATIONS: $$[QT_INSTALL_TRANSLATIONS]--")
message( "Ned file from $$[QT_INSTALL_PREFIX]/src/3rdparty/zlib/ ")



} else {
message("Oh!!!!!! you NOT! intall qt CORE Support, edit file:")
message( "--$${_DATE_}---------------------")
message(File $${_FILE_} :Line  $${_LINE_})
message( "--$${_DATE_}---------------------")
message( "PWD: $$PWD --")
message( "QT_VERSION: $$[QT_VERSION]--")
message( "QMAKE_MKSPECS: $$[QMAKE_MKSPECS]--")
message( "QT_INSTALL_LIBS: $$[QT_INSTALL_LIBS]--")
message( "QT_INSTALL_CONFIGURATION: $$[QT_INSTALL_CONFIGURATION]--")
message( "QT_INSTALL_DATA: $$[QT_INSTALL_DATA]--")
message( "QT_INSTALL_DOCS: $$[QT_INSTALL_DOCS]--")
message( "QT_INSTALL_PLUGINS: $$[QT_INSTALL_PLUGINS]--")
message( "QT_INSTALL_PREFIX: $$[QT_INSTALL_PREFIX]--")
message( "QT_INSTALL_TRANSLATIONS: $$[QT_INSTALL_TRANSLATIONS]--")
error("Not posibel to build...")
exit()
}


macx {
        #QMAKE_CFLAGS_X86_64 += -Xarch_x86_64 -mmacosx-version-min=10.5
        #QMAKE_CFLAGS_PPC_64 += -Xarch_ppc64 -mmacosx-version-min=10.5
}
