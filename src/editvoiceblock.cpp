/*
    Copyright (C)  2017 Piter K. <pehohlva@gmail.com>

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    History This file was a part from my own projects
    https://sourceforge.net/projects/wysiwyg-edit/
    https://code.google.com/archive/p/fop-miniscribus/
    old Link https://de.wikipedia.org/wiki/Extensible_Stylesheet_Language_%E2%80%93_Formatting_Objects
    XSL-FO Wysiwyg MiniScribus / XHTML Qeditor
    in 2008 - 2013 adapted from QT4.8 to QT5.9
*/
#include "editvoiceblock.h"
#include <QProgressDialog>
#include "core_htmldriver.h"
#include "voiceprocesing.h"

/*  make numeric a QString summ from unicode used plus year code..
 its faster to find in html code e pages.. */
static inline int genkeyname(const QString name) {
  int base = 0;
  int xsize = name.size();
  if (xsize > 55) {
    xsize = 54;
  }
  for (int o = 0; o < xsize; o++) {
    const QChar vox(name.at(o));
    const int unico = (int)vox.unicode();
    base += unico;
  }
  base += 2017;
  return base;
}

#include <QObject>



ToWorker::ToWorker(QObject *parent) : QObject(parent) { }

void ToWorker::run(const QStringList cmd ) {
    process = new QProcess();
    QString tmp = QString("say %1").arg(cmd.join(" "));
    SESSDEBUG() << __FUNCTION__ << " exec -  " << tmp;
    process->setReadChannelMode(QProcess::MergedChannels);
     connect(process, SIGNAL(finished(int)), this, SLOT(ready(int)));
     process->start("say",cmd, QIODevice::ReadOnly);
}

void ToWorker::stop() {
   process->kill();
}

void ToWorker::ready(int id ) {
   SESSDEBUG() << __FUNCTION__ << " WORKER TELL MEE ready-  " << id;
   emit targetVoiceReady(id);
}

static const int speedchaine = 1500;

VoiceBlock::VoiceBlock(QObject *parent) : QObject(parent) {
    now = pause;
    voicejob = new ToWorker(this);
    connect(voicejob, SIGNAL(targetVoiceReady(int)), this, SLOT(speechEnd(int)));
}




void VoiceBlock::killVoice() {
         now = pause;
         stateB=0;
         if (!voicejob) {
            voicejob = new ToWorker(this);
         } else {
            voicejob->deleteLater();
            voicejob = new ToWorker(this);
         }
         connect(voicejob, SIGNAL(targetVoiceReady(int)), this, SLOT(speechEnd(int)));
         voicejob->stop();
}

void VoiceBlock::say(const QString text , Vstate e ) {
    if (now == speeach && e !=internuse ) {
        return;
    }
  if (e == internuse && stateB ==0) {
      stateB =1;
  }
  SESSDEBUG() << __FUNCTION__ << " - txt to read size->" << text.size();
  now = speeach;
  QString filetextnow = CACHESPEECHFILE;
  if (stateB > 0) {
     filetextnow =    CACHEBUFFERDISKTMP + QString("/readblock_%1.dat").arg(stateB);
  }
  unlink(filetextnow);
  QFile f(filetextnow);
  const int tc = text.size();
  bool password = false;
  if (f.open(QFile::Truncate | QFile::Text | QFile::ReadWrite)) {
    QTextStream sw(&f);
    sw.setCodec(QTextCodec::codecForName("UTF-8"));
    sw << text;
    f.close();
    const qint64 bytetext = f.bytesAvailable();
    if (bytetext > 0) {
       SESSDEBUG() << __FUNCTION__ << " XXXXXXXXXX - txt to read size->" << bytetext;
      password = true;
    }
  }
  if ( password && currentVoice.IDVoice > 0 ) {
    QStringList cmd;
    cmd << QString("-f");
    cmd << filetextnow;
    cmd << QString("-v");
    cmd << currentVoice.voicename;
    ///// cmd << QString("--quality=125");
    SESSDEBUG() << __FUNCTION__ << " - launch voice ->" << cmd << "-" << currentVoice.debug();
    voicejob->run(cmd);
  } else {
    stopfast();
    SESSDEBUG() << __FUNCTION__ << " - STOP XXXXXXXX -" << currentVoice.debug();
    emit setVoicePriorMessage(QString("Unable to write inside Cache dir... disk is full?"));
  }
}

void VoiceBlock::sayDemoVoice() {
  const int uservoice = DOC::self(this)->value("MyVoicePref").toInt();
  Voice person = TakeVoiceId(uservoice);
  if (!person.voicename.isEmpty()) {
    currentVoice = person;
    QString text = person.demotext.replace(QString("#"), QString());
    say(text,outside);
    SESSDEBUG() << __FUNCTION__ << " - " << person.debug();
  } else {
    SESSDEBUG() << __FUNCTION__ << " - Assert null person!!!!!!XXXXXXXXXX";
  }
}

Voice VoiceBlock::TakeVoiceId(const int pref) {
  ////  -f file.txt -v voicename -i, --interactive    ... say -f read.dat -v
  ///Federica
  QList<Voice>::const_iterator x;
  for (x = voices.constBegin(); x != voices.constEnd(); ++x) {
    Voice fox = *x;
    if (pref == fox.IDVoice) {
      return fox;
    }
  }
  Voice nullio;
  SESSDEBUG() << __FUNCTION__ << " grrrr not found id go assert!";
  return nullio;
}

void VoiceBlock::FillvaiableVoice() {
  voices.clear();
  QStringList cmd; //// mac comand ( say -v "?" )
  cmd << QString("-v");
  cmd << QString("?");
  const QString litflat = say_comand_sdout(cmd);
  if (litflat.size() > 0) {
    QStringList linebyline = litflat.split(QString("\n"));
    for (int x = 0; x < linebyline.size(); x++) {
      QString liner = linebyline.at(x);
      QString name = liner.left(20).trimmed();
      int su = liner.indexOf(QString("#"));
      QString country = liner.mid(20, 12).trimmed();
      if (country.contains(QString("#")) != -1) {
        country = liner.mid(20, 6).trimmed();
      }
      QLocale st(country);
      Voice one;
      one.voicename = name;
      one.language = st.nativeLanguageName();
      one.languageID = (int)st.language();
      one.countryname = st.countryToString(st.country());
      QString comment = liner.mid(su - 1, liner.size() - su).trimmed();
      int xid = genkeyname(name + one.language + one.countryname);
      one.IDVoice = xid;
      one.demotext = comment;
      if (!name.isEmpty()) {
        voices.append(one);
        SESSDEBUG() << __FUNCTION__ << " refill " << one.debug();
      }
    }
  }
}

QString VoiceBlock::say_comand_sdout(QStringList comandlist) {
  /////  QString tmp= QString("say %1").arg(comandlist.join(" "));
  /////SESSDEBUG() << __FUNCTION__ << " exec -  " << tmp;
  QString lstr;
  QString cmd = QString("say");
  QProcess *process = new QProcess(this);
  process->setReadChannelMode(QProcess::MergedChannels);
  process->start(cmd, comandlist, QIODevice::ReadOnly);
  if (!process->waitForFinished()) {
    lstr = QString();
  } else {
    lstr = QString(process->readAll().constData());
    if (lstr.isEmpty()) {
      emit setDumpMessage(
          QString("End Read from Voice %1").arg(currentVoice.voicename));
    }
  }
  return lstr;
}

/*
take voice cmd:
say -v "?"
say -o out22.aiff -v Federica "Buongiorno a tutti come state, avete iniziato
bene la giornata?"


*/

void VoiceBlock::formatBlok(const int blokid, bool highlight) {

  QTextBlock block = edit->document()->findBlockByNumber(blokid);
  if (!block.isValid()) {
    return;
  }
  QTextBlockFormat sfo = block.blockFormat();
  QTextCursor cu(block);
  QColor baks(110, 218, 230, 50);
  if (highlight) {
    sfo.setBackground(QBrush(baks, Qt::SolidPattern));
    cu.setBlockFormat(sfo);
  } else {
    QBrush tt(Qt::transparent, Qt::SolidPattern);
    QBrush xx = sfo.brushProperty(QTextBlockFormat::BackgroundBrush);
    if (xx != tt) {
      sfo.setBackground(tt);
      cu.setBlockFormat(sfo);
    }
  }
}

void VoiceBlock::sendursorTo(const int blocknr) {
  QTextBlock dd = edit->document()->findBlockByNumber(blocknr);
  if (dd.isValid()) {
    QTextCursor c(dd);
    const int xxdd = c.positionInBlock();
    c.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
    edit->setTextCursor(c);
  }
}

/*
void VoiceBlock::say() {
  if (now == speeach) {
    SESSDEBUG() << __FUNCTION__ << " - on speeach- ";
    QTimer::singleShot(speedchaine, this, SLOT(next()));
  } else {
    SESSDEBUG() << __FUNCTION__ << " - on pause or stop - ";
  }
}
 */
void VoiceBlock::stopfast() {
  if (now != stop) {
    now = stop;
    summtotblock = edit->document()->blockCount();
    for (int x = 0; x < summtotblock; x++) {
      formatBlok(x, false); //// clean my format if have..
    }
     sendursorTo(0);
     killVoice(); /// voice stop
     emit setVoicePriorMessage(QString("Stop Read Document.."));
  }
}

void VoiceBlock::next() {
  if (now == stop) {
    return;
  }
  summtotblock = edit->document()->blockCount();
  if (stateB !=1) {
  stateB++;
  }
  SESSDEBUG() << __FUNCTION__ << " - pos -> " << stateB;
  SESSDEBUG() << stateB << " - call next - " << now;
  if (stateB < summtotblock) {
    jump_and_Speeach(stateB);
  } else {
    formatBlok(stateB, false);
    formatBlok(stateB - 1, false);
    now = stop;
    sendursorTo(0);
    emit endreadPage();
    emit setDumpMessage(QString("End Read Document.."));
  }
}

void VoiceBlock::speechEnd( int sig ) {
    SESSDEBUG() << __FUNCTION__ << " ready-  " << sig << ":state" << stateB;
    now = pause;
     ///// if read paragraph continue line
     if ( stateB != 0) {
         now = speeach;
         next();
     }
}

void VoiceBlock::jump_and_Speeach(int blocknr) {
   SESSDEBUG() << __FUNCTION__ << " - pos -> " << blocknr;
  /*
  if (speechDoc->state() == QTextToSpeech::Speaking) {
      speechDoc->stop();
  }
  */
  if (now != speeach) {
    return;
  }
  QTextBlock dd = edit->document()->findBlockByNumber(blocknr);
  SESSDEBUG() << __FUNCTION__ << ":" << __LINE__ <<  " " << blocknr << dd.isValid();

  if (dd.isValid()) {
    const QString txt = dd.text();
    QString pobs = txt.trimmed();
            if (pobs.size() > 0) {
              formatBlok(blocknr, true);
              ////SESSDEBUG() << blocknr << " - pos - " << txt;
            } else {
              formatBlok(blocknr, false);
            }
    formatBlok(blocknr - 1, false);
    //// send cursor next block...to view text ok...
    QTextCursor bcu(dd);
    sendursorTo(blocknr + 1);
            if (txt.isEmpty()) {
              SESSDEBUG() << __FUNCTION__ << ":" << __LINE__ <<  " " << blocknr << " go next";
              next();
              return;
            } else {
              SESSDEBUG() << __FUNCTION__ << ":" << __LINE__ << now <<  txt.size() << " " << blocknr << " go say";
              say(txt,internuse);
              return;
            }
  }
  SESSDEBUG() << __FUNCTION__ << " - call - novalid block" << blocknr;
}

void VoiceBlock::init_on(QTextEdit *e) {
  if (now == speeach) {
    return; /// call stop to destroy...
  }
  edit = e;
  QString fillt = e->document()->toPlainText();
  if (fillt.size() < 10) {
    now = stop;
    emit endreadPage();
    emit setDumpMessage(QString("No Text found to Speeck or no Documents."));
    return;
  }
  int beginbb = 12;
  stateB = 0;
  const int gg =
      DOC::self(this)->value(QString("DoumentCurrentLanguage")).toInt();
  summtotblock = edit->document()->blockCount();
  const QString engine = QString("default");
  QTextCursor ecu = edit->textCursor();
  now = speeach;
  jump_and_Speeach(0);
}
