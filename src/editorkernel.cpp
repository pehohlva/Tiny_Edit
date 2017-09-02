#include "editorkernel.h"
#include "oasimain.h"
#include <QImage>
#include <QTextBrowser>
#include <Qfile>

#include "editortable_setting.h"
#include "imageedit.h"
#include "ui_forms.h"

EditorKernel::EditorKernel() : QTextBrowser() {
  edit_yes = true;
  active_or_not = 10;
  modus_edit(edit_yes);
  this->setContentsMargins(0, 0, 0, 0);
  screenprint = new QShortcut(QKeySequence(tr("Ctrl+W", "Print Screen")), this);
  connect(screenprint, SIGNAL(activated()), this, SLOT(MakePrintScreen()));
  //// this->document();
}

void EditorKernel::switchEditModus() {
  if (edit_yes) {
    edit_yes = false;
    modus_edit(false);
  } else {
    edit_yes = true;
    modus_edit(false);
  }
}

void EditorKernel::modus_edit(bool e) {
  if (e) {
    //// edit...
    setOpenExternalLinks(false);
    setOpenLinks(false);
    setOverwriteMode(true);
    setReadOnly(false);
  } else {
    /// view
    setOpenExternalLinks(true);
    setOpenLinks(true);
    setOverwriteMode(false);
    setReadOnly(true);
  }
}

void EditorKernel::insertImage(const QString filenew) {
  QImage xpic;
  xpic.load(filenew);
  if (!xpic.isNull()) {
    QDateTime timer1(QDateTime::currentDateTime());
    const qint64 sec1970 = timer1.currentMSecsSinceEpoch();
    QString randomname = QString::number(sec1970) + QString(".png");
    QTextImageFormat format;
    format.setWidth(xpic.width());
    format.setHeight(xpic.height());
    format.setName(randomname);
    this->textCursor().insertImage(format);
    this->document()->addResource(QTextDocument::ImageResource,
                                  QUrl(randomname), xpic);

    QFile fips(TMPIMAGE);
    if (fips.exists()) {
      ///// fips.remove();
    }

  } else {
    qDebug() << "### pic xpic.isNull()-> " << xpic.isNull();
  }
}

void EditorKernel::Image_mod_Setting() {
  QFile fips(TMPIMAGE);
  if (fips.exists()) {
    fips.remove();
  }
  const QString picname = nowimage.name();
  if (picname.isEmpty()) {
    return;
  }
  QDateTime timer1(QDateTime::currentDateTime());
  QVariant img = this->document()->resource(QTextDocument::ImageResource,
                                            picname); //// get image
  QImage imgxx = img.value<QImage>();
  qDebug() << "### picnamepicnamepicnamepicnamepicnamepicname origin  "
           << imgxx.width();
  imgxx.save(TMPIMAGE, "PNG", 9);
  if (nowimage.isValid()) {
    Interface::self(this)->SetFormat(nowimage);
    Interface::self(this)->SetPic(TMPIMAGE);
    Interface::self(this)->exec();

    QImage imgswap;
    imgswap.load(TMPIMAGE, "PNG");
    qDebug() << "### picnamepicnamepicnamepicnamepicnamepicname swap  "
             << imgswap.width();
    QTextImageFormat newforminepic = Interface::self(this)->GetFormat();
    if (newforminepic.isValid()) {
      if (!imgswap.isNull()) {
        const qint64 sec1970 = timer1.currentMSecsSinceEpoch();
        QString randomname = QString::number(sec1970) + QString(".png");
        QTextImageFormat format;
        format.setWidth(imgswap.width());
        format.setHeight(imgswap.height());
        format.setName(randomname);
        this->textCursor().insertImage(format);
        this->document()->addResource(QTextDocument::ImageResource,
                                      QUrl(randomname), imgswap);
      } else {
        qDebug() << "###  image is null ";
      }
      /////// insertImage(TMPIMAGE); //// become new name..
      ///// QTimer::singleShot(200, this, SLOT(RepaintScreen()));
    }
  }
}

void EditorKernel::contextMenuEvent(QContextMenuEvent *event) {
  QMenu *menu = this->createStandardContextMenu();

  menu->addSeparator();

  QPixmap pix(22, 22);
  pix.fill(Qt::black);
  bool Eimage = false;
  QTextCursor c = this->textCursor();
  lastcursor = this->textCursor();

  QTextBlock textblocc = lastcursor.block();

  Eframe = lastcursor.currentFrame();
  nowimage = lastcursor.charFormat().toImageFormat();
  nowtable = lastcursor.currentTable();
  Eimage = nowimage.isValid();
  Etable = lastcursor.currentTable();
  isqtextblok = textblocc.isValid();

  if (nowimage.isValid()) {
    QString picname = nowimage.name();
    qDebug() << "### picnamepicnamepicnamepicnamepicnamepicname  " << picname;
    QFileInfo locinfo(picname);
    menu->addAction(
        tr("Image edit \"%1\" width - height").arg(locinfo.fileName()), this,
        SLOT(Image_mod_Setting()));
  }

  tableContext = new QMenu(tr("Table Option"), this);
  tableContext->setIcon(QIcon(QString::fromUtf8(":/img/table.png")));
  tableContext->addAction(tr("Insert Table here"), this,
                          SLOT(CreateanewTable()));

  menu->addMenu(tableContext);

  if (lastcursor.currentTable()) {
    tableContext->addAction(
        QIcon(QString::fromUtf8(":/img/table.png")),
        tr("Table (this) Propriety BackgroundColor, Padding, Spacing"), this,
        SLOT(TableSetting()));
    tableContext->addAction(QIcon(QString::fromUtf8(":/img/row_table.png")),
                            tr("Merge selected cell (if select)"), this,
                            SLOT(MergeCellByCursorPosition()));
    tableContext->addAction(tr("Append Row on this table"), this,
                            SLOT(AppendTableRows()));
    tableContext->addAction(tr("Append Cools on this table"), this,
                            SLOT(AppendTableCools()));
    tableContext->addAction(tr("Remove this row"), this,
                            SLOT(RemoveRowByCursorPosition()));
    tableContext->addAction(tr("Remove this cool"), this,
                            SLOT(RemoveCoolByCursorPosition()));
    tableContext->addAction(tr("Set Cell backgroundColor"), this,
                            SLOT(SetTableCellColor()));
    tableContext->addAction(tr("Set Cell Width"), this, SLOT(SetColumLarge()));
  }
  qDebug() << "### xxxx  " << event->globalPos();
  menu->popup(event->globalPos());
}

void EditorKernel::MakePrintScreen() {
  bool ok;
  int i = QInputDialog::getInt(
      0, tr("Wait Sec. to PrintScreen...."),
      tr("Wait Sec. to PrintScreen....<br>Sec: from 1/10"), 1, 1, 10, 1, &ok);
  if (ok && i > 0 && i < 11) {
    QTimer::singleShot(i * 1000, this, SLOT(PrintScreen()));
  }
}

void EditorKernel::insertFromMimeData(const QMimeData *source) {
  if (source->hasImage()) {
    active_or_not++;
    QDateTime timer1(QDateTime::currentDateTime());
    const qint64 secsince70 = timer1.currentMSecsSinceEpoch();
    const QString localimage =
        QString("%2/image_%1.png").arg(secsince70).arg(QDir::homePath());
    QImage image = qvariant_cast<QImage>(source->imageData());
    bool salvato = image.save(localimage, "PNG", 100);
    if (salvato) {
      emit TakeImage(localimage);
      QUrl urlpics = QUrl::fromLocalFile(localimage);
      document()->addResource(QTextDocument::ImageResource, urlpics,
                              QVariant(image));
    }
  } else if (source->formats().contains("text/html")) {
    QString draghtml = source->html();
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(draghtml);
    textCursor().insertFragment(fragment);
    emit IncommingHTML(); //// tell to parent...
    return;
  } else {
    QTextEdit::insertFromMimeData(source);
  }
}

void EditorKernel::PrintScreen() {
  QFile fips(TMPIMAGE);
  if (fips.exists()) {
    fips.remove();
  }
  QDesktopWidget *dw = QApplication::desktop();
  QPixmap desktopscreen =
      QPixmap::grabWindow(dw->winId(), 0, 0, dw->width(), dw->height());
  desktopscreen.save(TMPIMAGE, "PNG", 100);
  insertImage(TMPIMAGE);
}

void EditorKernel::insertHtml(QString html) {
  QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(html);
  this->textCursor().insertFragment(fragment);
}

void EditorKernel::mouseDoubleClickEvent(QMouseEvent *e) {
  emit DoubleClick_is_now();

  if (lastcursor.currentTable()) {
    TableSetting();
    return;
  }

  QTextEdit::mouseDoubleClickEvent(e);
}

void EditorKernel::keyPressEvent(QKeyEvent *e) {

  QTextBrowser::keyPressEvent(e);
}

void EditorKernel::CreateanewTable() {

  QString subtext, collx, rowx;
  bool ok;
  int colonne = QInputDialog::getInt(this, tr("New Table cool"), tr("Cool:"), 3,
                                     1, 10, 1, &ok);
  int righe = QInputDialog::getInt(this, tr("New Table row"), tr("Row:"), 3, 1,
                                   100, 1, &ok);
  int largo = QInputDialog::getInt(this, tr("New Table percent on page"),
                                   tr("Percent \%:"), 100, 10, 100, 1, &ok);
  if (colonne > 0 and righe > 0 and largo > 0) {
    QStringList tables;
    tables.clear();
    tables.append(
        QString("<table border=\"1\" align=\"center\" width=\"%1%\" "
                "cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#ffffff\">")
            .arg(largo));
    for (int i = 0; i < righe; i++) {
      tables.append(QString("<tr>"));
      for (int o = 0; o < colonne; o++) {
        tables.append(QString("<td><p></p></td>"));
      }
      tables.append(QString("</tr>"));
    }
    tables.append(QString("</table>"));

    subtext = tables.join("\n");
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(subtext);
    this->textCursor().insertFragment(fragment);
  }
}

void EditorKernel::MergeCellByCursorPosition() {
  QTextCursor findercursor(this->textCursor());
  Etable = findercursor.currentTable();
  if (Etable) {
    nowtable->mergeCells(findercursor);
  }
}

void EditorKernel::RemoveRowByCursorPosition() {
  QTextCursor findercursor(this->textCursor());
  Etable = findercursor.currentTable();
  if (Etable) {
    QTextTableCell existingcell = nowtable->cellAt(findercursor);
    int cellcoolcursoris =
        existingcell.column();                /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    nowtable->removeRows(cellrowcursoris, 1);
  }
}

void EditorKernel::RemoveCoolByCursorPosition() {
  QTextCursor findercursor(this->textCursor());
  Etable = findercursor.currentTable();
  if (Etable) {
    QTextTableCell existingcell = nowtable->cellAt(findercursor);
    int cellcoolcursoris =
        existingcell.column();                /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    nowtable->removeColumns(cellcoolcursoris, 1);
  }
}

void EditorKernel::AppendTableRows() {
  bool ok = false;
  QTextCursor findercursor(this->textCursor());
  Etable = findercursor.currentTable();
  if (Etable) {
    QTextTableCell existingcell = nowtable->cellAt(findercursor);
    int cellcoolcursoris =
        existingcell.column();                /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    int approwtot = QInputDialog::getInt(this, tr("Append  line row"),
                                         tr("Row:"), 1, 1, 100, 1, &ok);
    if (ok and approwtot > 0) {
      nowtable->insertRows(cellrowcursoris + 1, approwtot);
    }
  }
}

void EditorKernel::AppendTableCools() {
  bool ok = false;
  QTextCursor findercursor(this->textCursor());
  Etable = findercursor.currentTable();
  if (Etable) {
    QTextTableCell existingcell = nowtable->cellAt(findercursor);
    int cellcoolcursoris =
        existingcell.column();                /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    int appcooltot = QInputDialog::getInt(this, tr("Table Column nr."),
                                          tr("Cool:"), 1, 1, 10, 1, &ok);
    if (ok and appcooltot > 0) {
      nowtable->insertColumns(cellcoolcursoris + 1, appcooltot);
    }
  }
}

void EditorKernel::TableSetting() {
  QTextCursor currentable = this->textCursor();
  Etable = currentable.currentTable();
  if (Etable) {
    Table_Setting::self(this)->SetFormat(nowtable);
    Table_Setting::self(this)->exec();
    QTextTableFormat newformine = Table_Setting::self(this)->GetNewFormats();
    if (newformine.isValid()) {
      nowtable->setFormat(newformine);
    }
  }
}

/*  fast & effektfull */
void EditorKernel::RepaintScreen() {
  QTextDocument *xdoc = new QTextDocument();
  const QString html = this->document()->toHtml("utf_8");
  QTextDocument *d = this->document();
  this->setDocument(xdoc);
  this->document()->setHtml(html);
  this->document()->adjustSize();
  QTextCursor c(this->document());
  c.beginEditBlock();
  switchEditModus(); /// reset to last modus..
}
