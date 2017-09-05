#ifndef OASI_APPLICATION_H
#define OASI_APPLICATION_H

#include <QCoreApplication>
#include <QApplication>
#include "oasimain.h"
#include "editorkernel.h"
#include "doc_session.h"

#define oasi  (static_cast<Oasi*>(QCoreApplication::instance()))

class Oasi : public QApplication
{
    Q_OBJECT
public:
    explicit Oasi( int &argc, char **argv );
    bool event (QEvent *event);

signals:
     void eventLog(QString);
     void signalFileOpen(QString);

public slots:
     void prepare_to_close();
     void loginternal(QString log);
private:
     OasiMain *mainwin;
     QString current_dir;

};

#endif // OASI_APPLICATION_H
