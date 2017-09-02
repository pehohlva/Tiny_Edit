#ifndef EDITORTABLE_SETTING_H
#define EDITORTABLE_SETTING_H

#include <QObject>
#include <QWidget>

#include <QActionGroup>
#include <QByteArray>
#include <QColorDialog>
#include <QDebug>
#include <QErrorMessage>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTextBlockFormat>
#include <QTextCodec>
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QTextFormat>
#include <QTextLength>
#include <QTextList>
#include <QTextListFormat>
#include <QTextStream>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>

#include "ui_forms.h"

/*  Save file as table_setting.h  */
/*  Class Table_Setting Created on Tue Sep 12 13:46:10 CEST 2006  */
//
#include <QPointer>
//
class Table_Setting : public QDialog, public Ui::Table_Setting {
  Q_OBJECT
  //
public:
  static Table_Setting *self(QWidget * = 0);
  void SetFormat(QTextTable *existingtable);
  QTextTableFormat GetNewFormats();
  //
protected:
  void closeEvent(QCloseEvent *);
  //
private:
  Table_Setting(QWidget * = 0);
  static QPointer<Table_Setting> _self;
  QTextTable *format_table;
  QTextTableFormat formtab;
  QTextLength trolengh;
  qreal xpadding;
  qreal xspacing;
  QBrush xbgcolor;
  int setfoundalign;
  int allineaesistente;
  qreal intborder;
  QColor normalcolor;
  QAction *TableColor;
  QPixmap pix;
  //
public slots:
  void ChoisColor();
  void UpdateFormats();
};

#endif // EDITORTABLE_SETTING_H
