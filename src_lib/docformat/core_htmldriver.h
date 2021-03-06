#ifndef CORE_HTMLDRIVER_H
#define CORE_HTMLDRIVER_H

#include <QtCore/qdatetime.h>
#include <QtCore/qendian.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qfile.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>
#include <qplatformdefs.h>
#include <QByteArray>
#include <QString>
#include <QProcess>
#include <QImage>
#include <QtCore>
#include <QtCore>
#include <QtCore>
#include <QtCore>


//// like on php
bool file_put_contents(const QString fullFileName, QString xml , int modus = 1);
QString readfile(const QString fullFileName );
bool is_text_tool();
bool is_file(const QString fullFileName);
bool unlink(const QString fullFileName);
bool copy(const QString inputfile, const  QString outfile);
void DownDir_RM(const QString d);
/* encode to name pics file ... */
QString encodeBase64( QString xml );
QString encodeBase64( QByteArray chunk );
/* decode to name pics file ...  */
QString decodeBase64( QString xml );
QString decodeBase64( QByteArray chunk );
/* check if a int or float nummer */
bool is_numeric(QString incomming);

// by error no image insert pix image transparent
#define __ONEPIXELGIF__ \
              QString("data:image/gif;base64,R0lGODlhAQABAAAAACH5BAEKAAEALAAAAAABAAEAAAICTAEAOw==")

#define CACHEBUFFERDISKTMP\
               QString( "%1/.kernelversion" ).arg(QDir::homePath())

#define CACHEFILETMP\
               QString( "%1/index.html" ).arg(CACHEBUFFERDISKTMP)

#define CACHESPEECHFILE\
               QString( "%1/read.dat" ).arg(CACHEBUFFERDISKTMP)

class HtmlDriver
{
public:
    HtmlDriver();
    ~HtmlDriver();
    void clean_cache();

private:
    int cacheop;
};

#endif // CORE_HTMLDRIVER_H
