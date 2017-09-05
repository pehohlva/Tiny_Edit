#ifndef OASIMAIN_H
#define OASIMAIN_H

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QColorDialog>
#include <QComboBox>
#include <QCoreApplication>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QSettings>
#include <QStatusBar>
#include <QTextCodec>
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QTextEdit>
#include <QTextList>
#include <QToolBar>
#include <QWidget>
#include <QtDebug>
#include <QtGui/QTextCharFormat>
#include <QtGui>
#include <QSystemTrayIcon>


#ifdef _PRINTERIOK_
#include <QPrinterInfo>
#include <QTextDocumentWriter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/qprintdialog.h>
#include <QtPrintSupport/qprinter.h>
#endif

#ifdef _HAVEXMLPARSE_
#include "qdom.h"
#include "qtxmlglobal.h"
#include "qtxmlversion.h"
#include "qxml.h"
#include <QtCore/qstring.h>
#include <QtXml/QtXmlDepends>
#include <QtXml/qtxmlglobal.h>
#endif

#ifdef _HAVINGNESONSPEECH_
#include <QTextToSpeech>
#include "editvoiceblock.h" /// compiler read block by block text
#endif


class VoiceBlock;
class EditorKernel;

class OasiMain : public QMainWindow {
  Q_OBJECT
public:
  explicit OasiMain(QWidget *parent = nullptr);
  ~OasiMain(void);

signals:
   void request_to_close();
public slots:
  void appsOpen(QString file);
  bool load(const QString &f);
  bool maybeSave();
  void setCurrentFileName(const QString &fileName);
  void fileNew();
  void fileOpen();
  bool fileSave();
  bool fileSaveAs();
  void filePrint();
  void filePrintPreview();
  void filePrintPdf();
  void textBold();
  void textUnderline();
  void textItalic();
  void textFamily(const QString &f);
  void textSize(const QString &p);
  void textStyle(int styleIndex);
  void textColor();
  void textAlign(QAction *a);
  void currentCharFormatChanged(const QTextCharFormat &format);
  void cursorPositionChanged();
  void clipboardDataChanged();
  void about();
  void printPreview(QPrinter *);
  void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
  void fontChanged(const QFont &f);
  void colorChanged(const QColor &c);
  void alignmentChanged(Qt::Alignment a);
  void DocumentChanged();

  ////
  void setTextStatus2( const QString txt );
  void setTextMsg( const QString txt );
  void TextOnlyTray(const QString txt);
  //// voice read block by block
  void runReadBlocks();
  void stopReadBlocks();



protected:
  QSystemTrayIcon *traytop;
  void drawall();
  void setupTextActions();
  void closeEvent (QCloseEvent *event);

  QAction *actionSave, *actionTextBold, *actionTextUnderline, *actionTextItalic,
      *actionTextColor, *actionAlignLeft, *actionAlignCenter, *actionAlignRight,
      *actionAlignJustify, *actionUndo, *actionRedo, *actionCut, *actionCopy,
      *actionPaste, *actionVoiceBlocks, *actionStopVoice;

  QStatusBar *statusbar;
  QFontComboBox *comboFont;
  QComboBox *comboSize, *comboStyle;
  QLabel *statustxt2, *statustxt;

  QToolBar *tb;
  QFileInfo currentin;
  QString fileName;
  EditorKernel *base_edit;
  QGridLayout *gridLayout;
  bool enableedit;
  int currdocsize;
  int firstdocsize;
#ifdef _HAVINGNESONSPEECH_
  VoiceBlock *vrspeak;
#endif


};

#endif // OASIMAIN_H
