
#include <QMainWindow>
#include <QCoreApplication>
#include <QApplication>
#include <QtGui>
#include <QFontDatabase>
///// #include "allconfig.h"
#include "oasimain.h"
#include "oasi_application.h"
#include "doc_session.h"

/////#include "allconfig.h"#include "mainwindow.h"*#include "core_application.h" #ifndef QT_NO_PRINTER QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
 ///// * Q_OS_MAC  Q_OS_WIN  Q_OS_LINUX  Q_OS_MAC*/

int main(int argc, char *argv[]) {
    Oasi a(argc, argv);
    return a.exec();
}



