#ifndef EDITORKERNEL_H
#define EDITORKERNEL_H

#include "oasimain.h"
#include <QObject>
#include <QTextBrowser>
#include "ui_forms.h"
#include "doc_session.h"

#define TMPIMAGE QString( "%1/zz_oasi_tmp_image.png" ).arg(QDir::homePath())



///// QTextBrowser or QTextEdit ?? big question
class EditorKernel : public QTextBrowser {
  Q_OBJECT
public:
     EditorKernel();
signals:
  void DoubleClick_is_now();
  void IncommingHTML();
  void TakeImage(QString);  /// remove image source
public slots:
  void RepaintScreen();
  void Image_mod_Setting();
  void PrintScreen();

  void MakePrintScreen();
  void CreateanewTable();
  void insertImage(const QString filenew);
  void insertHtml(QString html);

  void MergeCellByCursorPosition();
  void RemoveRowByCursorPosition();
  void RemoveCoolByCursorPosition();
  void AppendTableRows();
  void AppendTableCools();
  void TableSetting();
  void SetTableCellColor();
  void SetColumLarge();

  void switchEditModus();
  void modus_edit( bool e );

  void CreateanewImage();
  void removeFormat();

public:
  QMenu *contenentContext;
  QMenu *tableContext;
  QMenu *imageContext;

  QTextImageFormat nowimage;
  QTextTable *nowtable;
  bool Eframe;
  bool Eimage;
  bool Etable;
  bool isqtextblok;
  QTextCursor lastcursor;
  int active_or_not;
  bool edit_yes;
  QShortcut *screenprint;
  ///// QShortcut *kurzeltastatur_1;

protected:
  void wheelEvent(QWheelEvent *event);
  void keyPressEvent(QKeyEvent *e);
  void mouseDoubleClickEvent(QMouseEvent *e);
  void contextMenuEvent(QContextMenuEvent *event);
  void insertFromMimeData(const QMimeData *source);
  bool gestureNative(QNativeGestureEvent * e);
  bool event(QEvent * e);
  qreal scaleFaktor;
  bool portrait_mode;
  qint64 lineTimer;
  qreal distanceswip;
};

#endif // EDITORKERNEL_H
