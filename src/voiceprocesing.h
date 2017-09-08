#ifndef VOICEPROCESING_H
#define VOICEPROCESING_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QProgressBar>
#include "editvoiceblock.h"
#include "worker.h"
#include <QDialog>
#include <QObject>
#include <QPointer>
#include <QProcess>
#include <QProgressBar>
#include <QSettings>
#include <QTextDocument>
#include <QWidget>

int genkeyname(const QString name);

class VoiceProcesing : public QDialog {
  Q_OBJECT

public:
  static VoiceProcesing *self(QWidget * = 0);
  ~VoiceProcesing();

  enum Action_Cycle { start, chance, stop, idle };

signals:
  void setValuePros(int);

public slots:
  void updateProgress();
  void startAction();
  void closeallcyle();
  void setVoiceatd(int voiceid);

private:
  VoiceProcesing(QWidget * = 0);
  static QPointer<VoiceProcesing> _self;
  void setVoices();
  QGridLayout *gridLayout;
  QLabel *label_2;
  QComboBox *voicenames;
  QProgressBar *progressBar;
  QLabel *label;
  QLineEdit *lineEdit;
  QPushButton *startaction;
  QPushButton *abortation;
  QLabel *label_3;
  QProgressBar *progreso;
  int cursorp;
  Action_Cycle cnow; ///
  QElapsedTimer timer;
};

#endif // VOICEPROCESING_H
