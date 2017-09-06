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

static inline void say_comand_fly(const QStringList comandlist) {
  QString tmp = QString("say %1").arg(comandlist.join(" "));
  SESSDEBUG() << __FUNCTION__ << " exec -  " << tmp;
  QString lstr;
  QString cmd = QString("say");
  QProcess *process = new QProcess();
  process->setReadChannelMode(QProcess::MergedChannels);
  process->start(cmd, comandlist, QIODevice::ReadOnly);
  if (!process->waitForFinished()) {
    /////
  } else {
    lstr = QString(process->readAll().constData());
    SESSDEBUG() << __FUNCTION__ << " out -> exec -  " << lstr;
  }
}

static const int speedchaine = 1500;

VoiceBlock::VoiceBlock(QObject *parent) : QObject(parent) { now = pause; }

/*     QProgressDialog dialog;
    dialog.setLabelText(QString("Progressing using %1
 thread(s)...").arg(QThread::idealThreadCount()));

    // Create a QFutureWatcher and connect signals and slots.
    QFutureWatcher<void> futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog,
 SLOT(reset())); QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher,
 SLOT(cancel())); QObject::connect(&futureWatcher,
 SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog,
 SLOT(setValue(int)));

 // Start the computation.
    futureWatcher.setFuture(QtConcurrent::map(vector, spin));

    // Display the dialog and start the event loop.
    dialog.exec();

    futureWatcher.waitForFinished();

    // Query the future to check if was canceled.
    qDebug() << "Canceled?" << futureWatcher.future().isCanceled(); */
void VoiceBlock::say(const QString text) {
  QFile f(CACHESPEECHFILE);
  bool password = false;
  if (f.open(QFile::Truncate | QFile::Text | QFile::ReadWrite)) {
    QTextStream sw(&f);
    sw.setCodec(QTextCodec::codecForName("UTF-8"));
    sw << text;
    f.close();
    if (f.bytesAvailable() > 0) {
      password = true;
    }
  }
  if (password) {
    QStringList cmd;
    cmd << QString("-f");
    cmd << CACHESPEECHFILE;
    cmd << QString("-v");
    cmd << currentVoice.voicename;
    cmd << QString("--quality=125");
    //// SESSDEBUG() << __FUNCTION__ << " - " << cmd;
    QFuture<void> f2 = QtConcurrent::run(say_comand_fly, cmd);
    ///// f2.waitForFinished();
    emit setDumpMessage(
        QString("Read from Voice %1").arg(currentVoice.voicename));
  } else {
    emit setDumpMessage(
        QString("Unable to write inside Cache dir... disk is full?"));
  }
}

void VoiceBlock::sayDemoVoice() {
  const int uservoice = DOC::self(this)->value("MyVoicePref").toInt();
  Voice person = TakeVoiceId(uservoice);
  if (!person.voicename.isEmpty()) {
    currentVoice = person;
    QString text = person.demotext.replace(QString("#"), QString());
    say(text);
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
      }
      //// SESSDEBUG() << one.debug();
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

void VoiceBlock::say() {
  if (now == speeach) {
    SESSDEBUG() << __FUNCTION__ << " - on speeach- ";
    QTimer::singleShot(speedchaine, this, SLOT(next()));
  } else {
    SESSDEBUG() << __FUNCTION__ << " - on pause or stop - ";
  }
}

void VoiceBlock::stopfast() {
  if (now != stop) {
    now = stop;
    summtotblock = edit->document()->blockCount();
    for (int x = 0; x < summtotblock; x++) {
      formatBlok(x, false); //// clean my format if have..
    }
    sendursorTo(0);
    emit endreadPage();
    emit setDumpMessage(QString("Stop Read Document.."));
  }
}

void VoiceBlock::next() {
  if (now == stop) {
    return;
  }
  summtotblock = edit->document()->blockCount();
  stateB++;
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

void VoiceBlock::jump_and_Speeach(int blocknr) {

  /*
  if (speechDoc->state() == QTextToSpeech::Speaking) {
      speechDoc->stop();
  }
  */
  if (now != speeach) {
    return;
  }

  QTextBlock dd = edit->document()->findBlockByNumber(blocknr);
  if (dd.isValid()) {
    const QString txt = dd.text();
    QString pobs = txt.trimmed();
    if (pobs.size() > 0) {
      formatBlok(blocknr, true);
      SESSDEBUG() << blocknr << " - pos - " << txt;
    } else {
      formatBlok(blocknr, false);
      SESSDEBUG() << blocknr << " NULLTXT!!!!!!!";
    }
    formatBlok(blocknr - 1, false);
    //// send cursor next block...to view text ok...
    QTextCursor bcu(dd);
    sendursorTo(blocknr + 1);

    if (txt.isEmpty()) {
      next();
      return;
    } else {
      say();
      return;
    }
    //// speechDoc->say(txt);
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
  connect(this, SIGNAL(speeachvoice(int)), this, SLOT(jump_and_Speeach(int)));
  //// speechDoc = new QTextToSpeech();
  ////const QString handleer = (SetLanguage(gg))? QString("speeach_found") :
  ///QString("Sorry language not found");  if (handleer.size() < 12) {
  now = speeach;
  jump_and_Speeach(0);
  ///}
}
