#ifndef DOC_SESSION_H
#define DOC_SESSION_H

#include "editorkernel.h"
#include <QObject>
#include <QTextDocument>
#include <QWidget>
#include <QSettings>

#define _BASICORG_ QString("QTClub")
#define _BASICDOMAINE_ QString("org.freeroad.ch")
#define _BASICTITLE_EDITOR_ QString("OasisEdit - Document Tiny Editor")
#define _CVERSION_ QString("Vr. 1.2")


static inline QString unixwhich(QString apps = QString("gs")) {
#ifdef Q_OS_WIN
  return QString();
#endif

  QStringList potential_paths;
  potential_paths.append("/usr/bin");
  potential_paths.append("/usr/local/bin");
  potential_paths.append("/sw/bin"); /* to use on mac as same */
  potential_paths.append("/opt/bin");
  QProcess *process = new QProcess(NULL);
  process->setReadChannelMode(QProcess::MergedChannels);
  QStringList env = process->systemEnvironment();
  env.replaceInStrings(QRegExp("^PATH=(.*)", Qt::CaseInsensitive),
                       "PATH=\\1;" + potential_paths.join(";"));
  process->setEnvironment(env);
  process->start(QString("which"), QStringList() << apps, QIODevice::ReadOnly);
  if (!process->waitForFinished()) {
    return QString();
  } else {
    QString finder = process->readAll().trimmed();
    if (finder.endsWith(apps, Qt::CaseInsensitive)) {
      return finder;
    } else {
      return QString();
    }
  }
}

class DOC : public QObject {
  Q_OBJECT

public:
  static DOC *self(QObject *parent = nullptr);
  QString
  GetHtml(const QString file); //// null if QTextDocument is ok or html no image
  QTextDocument *GetQdoc(const QString file);
  bool haveBase64pics(const QString html);
  //// html + base64 encodet image go normal to QTextDocument back
  void HtmlDecode(QString &html, QTextDocument *d);
  /// portable images inside. & read ok.. browser & here.
  void DecodeHtml(QString &html, QTextDocument *d);
  ///// void setDoc(QTextDocument *indoc) { picsdoc = indoc->clone(); }
  QByteArray picxencode(QImage im);
  QString readcfile(const QString fullFileName);
signals:
  void setMessage(QString);
  void setPriorMessage(QString);
  void setMessageBase(QString);
public slots:
  void incommingText( QString txt );
  void log_documents( QString xtcr );
  void save();
private slots:
  void observerDoc();

private:
  DOC(QObject *parent = nullptr);
  static QPointer<DOC> _self;
  void run_textutils(const QString file);
  void image_grep(QString &html, QString &newhtml);
  QTextDocument *d;
  QTextDocument *picsdoc;
  int action;
  QString txtutils;
  QString msg;
  QString f;
};

#endif // DOC_SESSION_H
