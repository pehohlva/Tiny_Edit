#include "voiceprocesing.h"

#include <QPointer>

/*  make numeric a QString summ from unicode used plus year code..
 its faster to find in html code e pages.. */
int genkeyname(const QString name) {
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

QPointer<VoiceProcesing> VoiceProcesing::_self = 0L;

VoiceProcesing *VoiceProcesing::self(QWidget *parent) {
  if (!_self)
    _self = new VoiceProcesing(parent);
  return _self;
}

VoiceProcesing::VoiceProcesing(QWidget *parent) : QDialog(parent) {
  cursorp = 0;
  cnow = idle;
  setObjectName(QStringLiteral("VoiceProcesing"));
  ////resize(509, 120);
  setMinimumSize(QSize(680,170));
  setMaximumSize(QSize(700,170));
  QWidget::setWindowFlags(Qt::WindowSystemMenuHint | Qt::CustomizeWindowHint | Qt::Sheet);
  QWidget::setWindowModality(Qt::ApplicationModal);

  gridLayout = new QGridLayout(this);
  gridLayout->setObjectName(QStringLiteral("gridLayout"));
  label_2 = new QLabel(this);
  label_2->setObjectName(QStringLiteral("label_2"));

  gridLayout->addWidget(label_2, 0, 0, 1, 1);

  voicenames = new QComboBox(this);
  voicenames->setObjectName(QStringLiteral("voicenames"));

  gridLayout->addWidget(voicenames, 0, 1, 1, 3);
  this->setVoices(); /// fill data here

  progreso = new QProgressBar(this);
  progreso->setValue(0);
  progreso->setMaximum(100);
  progreso->setMinimum(0);

  gridLayout->addWidget(progreso, 1, 0, 1, 4);

  label = new QLabel(this);
  label->setText(QStringLiteral("Filename:"));
  label->setObjectName(QStringLiteral("label"));

  gridLayout->addWidget(label, 2, 0, 1, 1);

  lineEdit = new QLineEdit(this);
  lineEdit->setObjectName(QStringLiteral("lineEdit"));

  lineEdit->setText(QString("AudioFile.aiff"));
  gridLayout->addWidget(lineEdit, 2, 1, 1, 1);

  startaction = new QPushButton(tr("&Start...."));
  //// startaction->setFlat(true);

  gridLayout->addWidget(startaction, 2, 2, 1, 1);

  abortation = new QPushButton(tr("&Abort - Cancell"));
  //// abortation->setFlat(true);

  gridLayout->addWidget(abortation, 2, 3, 1, 1);

  label_3 = new QLabel(this);
  label_3->setObjectName(QStringLiteral("label_3"));
  QString settime = QString("%1").arg(QTime(0,0).addMSecs(1).toString());
  label_3->setText(settime);
  label_3->setFixedWidth(77);
  gridLayout->addWidget(label_3, 2, 4, 1, 1);

  this->setLayout(gridLayout);
  QObject::connect(startaction, SIGNAL(clicked()), this, SLOT(startAction()));
  QObject::connect(abortation, SIGNAL(clicked()), this, SLOT(closeallcyle()));
  setWindowTitle(tr("VoiceProcesing"));
  QObject::connect(this, SIGNAL(setValuePros(int)), progreso,
                   SLOT(setValue(int)));
  QObject::connect(voicenames, SIGNAL(activated(int)), this, SLOT(setVoiceatd(int)));
}

void VoiceProcesing::startAction() {
  timer.start();
  progreso->setValue(0);
  voicenames->setEnabled(false);
  lineEdit->setModified(false);
  lineEdit->setDisabled(true);
  startaction->setDisabled(true);
  cnow = start;
  cnow = chance;
  timer.restart();
  updateProgress();
}

void VoiceProcesing::updateProgress() {

  if (cnow == stop) {
    return; /// end transform
  }
  cursorp++;
  if (cursorp == 101) {
    cursorp = 0;
  }
  qint64 tt = timer.elapsed();

  ///// int secx = tt/1000;
  QString settime = QString("%1").arg(QTime(0,0).addMSecs(tt).toString());
  label_3->setText(settime);
  const int timeintervall = 20;
  QTimer::singleShot(timeintervall, this, SLOT(updateProgress()));
  emit setValuePros(cursorp);
}

void VoiceProcesing::closeallcyle() {
  cnow = stop;
  timer.restart();
  progreso->setValue(0);
  voicenames->setEnabled(true);
  lineEdit->setDisabled(false);
  lineEdit->setModified(true);
  startaction->setDisabled(false);
  this->close();
}


void VoiceProcesing::setVoiceatd(int voiceid) {
  voicenames->setEnabled(false);
   lineEdit->setModified(true);
  const int myvoice = voicenames->itemData(voiceid).toInt();
  Voice pv = VoiceBlock::self(this)->TakeVoiceId(myvoice);
  if (!pv.voicename.isEmpty()) {
  lineEdit->setText(QString("Audio_%1_File").arg(pv.voicename));
  } else {
    lineEdit->setText(QString("Error here no name from voice."));
  }
  voicenames->setEnabled(true);
}

void VoiceProcesing::setVoices() {
    QList<Voice> vitem = VoiceBlock::self(this)->avaiableVoices();
    QList<Voice>::const_iterator x;
    voicenames->clear();
    const int localvoi = DOC::self(this)->value("SYSTEMLOCALEVOICE").toInt(); /// register any load systemlocale
    int uservoice = DOC::self(this)->value("MyVoicePref").toInt();
    if (uservoice < 10) {
        uservoice = localvoi;
    }
    for (x = vitem.constBegin(); x != vitem.constEnd(); ++x) {
      Voice fox = *x;
      QString name = QString("%1/%2 - %3")
                         .arg(fox.voicename)
                         .arg(fox.countryname)
                         .arg(fox.language);
      name.append(QString("-%1").arg(fox.languageID));
      voicenames->addItem(name, QVariant(fox.IDVoice));
      if (uservoice == fox.IDVoice) {
        voicenames->setCurrentIndex(voicenames->count() - 1);
      }
    }

}


VoiceProcesing::~VoiceProcesing() {}
