#include "editvoiceblock.h"

#ifdef _HAVINGNESONSPEECH_
#include <QTextToSpeech>

static const int speedchaine = 1500;

VoiceBlock::VoiceBlock(QObject *parent) : QObject(parent)
{
  now = pause;
}

void VoiceBlock::formatBlok(const int blokid, bool highlight ) {

    QTextBlock block = edit->document()->findBlockByNumber(blokid);
    if (!block.isValid()) {
        return;
    }
    QTextBlockFormat sfo = block.blockFormat();
    QTextCursor cu(block);
    QColor baks(110,218,230,50);
    if (highlight) {
    sfo.setBackground( QBrush(baks,Qt::SolidPattern));
    cu.setBlockFormat(sfo);
    } else {
        QBrush tt(Qt::transparent,Qt::SolidPattern);
        QBrush xx = sfo.brushProperty(QTextBlockFormat::BackgroundBrush);
        if (xx !=tt) {
              sfo.setBackground(tt);
              cu.setBlockFormat(sfo);
        }
   }
}


void VoiceBlock::sendursorTo(const int blocknr ) {
    QTextBlock dd = edit->document()->findBlockByNumber(blocknr);
    if (dd.isValid()) {
        QTextCursor c(dd);
        const int xxdd = c.positionInBlock();
        c.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
        edit->setTextCursor(c);
    }
}

void VoiceBlock::say() {
    if (now == run) {
    SESSDEBUG() << __FUNCTION__ << " - on run- ";
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
              formatBlok(x,false); //// clean my format if have..
            }
       sendursorTo(0);
       emit endreadPage();
       emit setVoicePriorMessage(QString("Stop Read Document.."));

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
        formatBlok(stateB,false);
        formatBlok(stateB - 1 ,false);
        now = stop;
        sendursorTo(0);
        emit endreadPage();
        emit setVoicePriorMessage(QString("End Read Document.."));
    }
}

void VoiceBlock::jump_and_Speeach(int blocknr ) {

    /*
    if (speechDoc->state() == QTextToSpeech::Speaking) {
        speechDoc->stop();
    }
    */
    if (now !=run) {
        return;
    }

    QTextBlock dd = edit->document()->findBlockByNumber(blocknr);
    if (dd.isValid()) {
        const QString txt = dd.text();
        QString pobs = txt.trimmed();
        if (pobs.size() > 0) {
        formatBlok(blocknr,true);
         SESSDEBUG() << blocknr << " - pos - " << txt;
        } else {
         formatBlok(blocknr,false);
          SESSDEBUG() << blocknr << " NULLTXT!!!!!!!";
        }
        formatBlok(blocknr - 1,false);
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





bool VoiceBlock::SetLanguage( const int id ) {
   bool ok = false;
   QVector<QLocale> loc = speechDoc->availableLocales();
   foreach (const QLocale &locale, loc) {
          const int langid = (int)locale.language();
          if (langid == id ) {
              speechDoc->setLocale(locale); /// job2
              ok = true;
          }
      }
return ok;
}


void VoiceBlock::init_on( QTextEdit *e ) {
    if (now == run) {
        return; /// call stop to destroy...
    }
   edit = e;
   QString fillt = e->document()->toPlainText();
   if (fillt.size() < 10) {
       now = stop;
       emit endreadPage();
       emit setVoicePriorMessage(QString("No Text found to Speeck or no Documents."));
       return;
   }
   int beginbb = 12;
   stateB = 0;
   const int gg = DOC::self(this)->value(QString("DoumentCurrentLanguage")).toInt();
   summtotblock = edit->document()->blockCount();
   const QString engine = QString("default");
   QTextCursor ecu = edit->textCursor();
   connect(this, SIGNAL(runvoice(int)), this, SLOT(jump_and_Speeach(int)));
   //// speechDoc = new QTextToSpeech();
   ////const QString handleer = (SetLanguage(gg))? QString("run_found") : QString("Sorry language not found");
   ///if (handleer.size() < 12) {
   now = run;
        jump_and_Speeach(0);
   ///}
}


#endif
