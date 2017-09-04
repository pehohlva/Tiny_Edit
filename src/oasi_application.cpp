#include "oasi_application.h"
#include "oasimain.h"
#include "doc_session.h"


Oasi::Oasi(int &argc, char **argv)
   : QApplication(argc,argv)
{

  /// file list in linux here
    QCoreApplication::setOrganizationName(_BASICORG_);
      QCoreApplication::setOrganizationDomain(_BASICDOMAINE_);
      QCoreApplication::setApplicationName(_BASICTITLE_EDITOR_);

  QString current_dir = this->applicationDirPath();
  mainwin = new OasiMain();
  mainwin->show();
  connect(this, SIGNAL(lastWindowClosed()),this, SLOT(quit()));
  connect(this, SIGNAL(signalFileOpen(QString)),mainwin,SLOT(appsOpen(QString))); //// mac
}


void Oasi::prepare_to_close() {
    qDebug() << "### prepare_to_close";
    QMessageBox::StandardButton resBtn = QMessageBox::question(mainwin,_BASICTITLE_EDITOR_,
                                                               tr("Are you sure to quit?\n"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    const int permission = (int)resBtn;
    if ( permission == 16384 ) {
        DOC::self(this)->save();
        //// sleep(2);
       QApplication::quit();
       return;
    }
    //// emit eventLog(tr("Continue to work..."));
}

/* only this class for mac drag file to icon in app status Dock  */
bool Oasi::event (QEvent *event) {
       if (event->type() == QEvent::FileOpen) {
       const QString  file_path = static_cast<QFileOpenEvent *> (event)->file();
       emit signalFileOpen(file_path);
       } else {
         return QApplication::event (event);
       }
}

void Oasi::loginternal(QString log)
{
   qDebug() << "### Oasi::loginternal >>" << log;
}
