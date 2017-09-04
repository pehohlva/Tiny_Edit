#include "oasimain.h"

#include <QToolBar>
#include <QMenu>
#include <QMainWindow>
#include <QMenuBar>
#include <QComboBox>
#include <QLabel>
#include <QSettings>
#include <QObject>




#ifndef _ODTREADON_
#include <OdtFormat>
#endif

#ifndef _RTFREADON_
#include <RTFFormat>
#endif

#include "editorkernel.h"
#include "doc_session.h"

#define _CVERSION_ \
             QString("Vr. 1.2")

#define __TMPCACHE__ \
             QString("%1/.fastcache/").arg(QDir::homePath())


//// default :/images/ODTicon.png
#ifdef Q_WS_MAC
static const QString rsrcPath = ":/images/mac";
#else
static const QString rsrcPath = ":/images/win";
#endif

static const QString _not_save_doc_ = QString("document.odt");

QSettings setter(QString("OasiClub"),QString("OasiEditor"));

static const int toolbarhight = 25;
static const QString xtyle = QString("");  ///// background-color: #c0d6d5;


bool OasiMain::load(const QString &f) {
    qDebug() << "load in:" << f;
    if (!enableedit) {
         //// return false;
    }
    QMimeDatabase mimeDatabase;
    QMimeType mimeType;
    mimeType = mimeDatabase.mimeTypeForFile(f);
    QStringList alii = mimeType.aliases();

    const QString html = DOC::self(this)->GetHtml(f); //// no images
    QTextDocument *xdoc = DOC::self(this)->GetQdoc(f); //// images clone here
    bool havedata = xdoc->isEmpty();
    if (html.isEmpty() && !havedata) {
        base_edit->setDocument(xdoc);
        setCurrentFileName(f);
        return true;
    } else {
        base_edit->setHtml(html);
        setCurrentFileName(f);
        return true;
    }

}


OasiMain::OasiMain(QWidget *parent) : QMainWindow(parent)
{
  this->setContentsMargins(5, 0, 5, 0);
  enableedit = false;
  setWindowIcon(QIcon(":/images/ODTicon.png"));
  setStyleSheet(xtyle);
  drawall();
  base_edit->document()->setUndoRedoEnabled(false);

  connect(base_edit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
          this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
  connect(base_edit, SIGNAL(cursorPositionChanged()), this,
          SLOT(cursorPositionChanged()));

  //// document session setPriorMessage
  connect(DOC::self(this), SIGNAL(setMessage(QString)), this, SLOT(setTextMsg(QString)));
  connect(DOC::self(this), SIGNAL(setPriorMessage(QString)), this, SLOT(setTextTray(QString)));
  fontChanged(base_edit->font());
  colorChanged(base_edit->textColor());
  alignmentChanged(base_edit->alignment());




  connect(base_edit->document(), SIGNAL(modificationChanged(bool)), actionSave,
          SLOT(setEnabled(bool)));
  connect(base_edit->document(), SIGNAL(modificationChanged(bool)), this,
          SLOT(DocumentChanged()));


  connect(base_edit->document(), SIGNAL(modificationChanged(bool)), this,
          SLOT(setWindowModified(bool)));
  connect(base_edit->document(), SIGNAL(undoAvailable(bool)), actionUndo,
          SLOT(setEnabled(bool)));
  connect(base_edit->document(), SIGNAL(redoAvailable(bool)), actionRedo,
          SLOT(setEnabled(bool)));

  setWindowModified(base_edit->document()->isModified());
  connect(actionUndo, SIGNAL(triggered()), base_edit, SLOT(undo()));
  connect(actionRedo, SIGNAL(triggered()), base_edit, SLOT(redo()));

  actionCut->setEnabled(false);
  actionCopy->setEnabled(false);
  actionRedo->setEnabled(true);
  actionUndo->setEnabled(true);

  connect(actionCut, SIGNAL(triggered()), base_edit, SLOT(cut()));
  connect(actionCopy, SIGNAL(triggered()), base_edit, SLOT(copy()));
  connect(actionPaste, SIGNAL(triggered()), base_edit, SLOT(paste()));

  connect(actionUndo, SIGNAL(triggered()), base_edit, SLOT(undo()));
  connect(actionRedo, SIGNAL(triggered()), base_edit, SLOT(redo()));

  connect(base_edit, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
  connect(base_edit, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));

  //// connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
  show();
  base_edit->document()->setUndoRedoEnabled(true);
  traytop->showMessage(QString("Banana.... "),QString("Le banane sono li ...."),QSystemTrayIcon::Warning,15000);
  setTextTray(QString());
}


void OasiMain::drawall() {

    QMenuBar *xtop = menuBar();
    QToolBar *tb = new QToolBar();
    tb->setMaximumHeight(toolbarhight);
    tb->setWindowTitle(tr("File Actions"));
    this->addToolBar(tb);
    QPixmap perimage(":/images/ODTicon.png");
   traytop = new QSystemTrayIcon(QIcon(perimage),this);
   traytop->setToolTip(tr("System tray sample "));
   traytop->setIcon(QIcon(perimage));
   traytop->setObjectName(tr("SystemTrayHandler"));
  QAction *maximizeAction = new QAction("Maximize Window", this);
  connect(maximizeAction,&QAction::triggered, this, &OasiMain::showMaximized );



      traytop->setVisible(true);

  QMenu *menu = new QMenu(tr("&File"), this);
  menuBar()->addMenu(menu);
  QAction *a;
  a = new QAction(QIcon(rsrcPath + "/filenew.png"), tr("&New Page"), this);
  a->setShortcut(QKeySequence::New);
  connect(a, SIGNAL(triggered()), this, SLOT(fileNew()));
  tb->addAction(a);
  menu->addAction(a);

  a = new QAction(QIcon(rsrcPath + "/fileopen.png"), tr("&Open..."), this);
  a->setShortcut(QKeySequence::Open);
  connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
  tb->addAction(a);
  menu->addAction(a);

  menu->addSeparator();

  actionSave = a =
      new QAction(QIcon(rsrcPath + "/filesave.png"), tr("&Save"), this);
  a->setShortcut(QKeySequence::Save);
  connect(a, SIGNAL(triggered()), this, SLOT(fileSave()));
  a->setEnabled(false);
  tb->addAction(a);
  menu->addAction(a);

  a = new QAction(tr("Save &As..."), this);
  connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
  menu->addAction(a);
  menu->addSeparator();

  a = new QAction(QIcon(rsrcPath + "/fileprint.png"), tr("&Print..."), this);
  a->setShortcut(QKeySequence::Print);
  connect(a, SIGNAL(triggered()), this, SLOT(filePrint()));
  tb->addAction(a);
  menu->addAction(a);

  a = new QAction(QIcon(rsrcPath + "/fileprint.png"), tr("Print Preview..."),
                  this);
  connect(a, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
  menu->addAction(a);

  a = new QAction(QIcon(rsrcPath + "/exportpdf.png"), tr("&Export PDF..."),
                  this);
  a->setShortcut(Qt::CTRL + Qt::Key_D);
  connect(a, SIGNAL(triggered()), this, SLOT(filePrintPdf()));
  tb->addAction(a);
  menu->addAction(a);

  menu->addSeparator();

  menu->addAction(maximizeAction);

  a = new QAction(tr("&Quit"), this);
  a->setShortcut(Qt::CTRL + Qt::Key_Q);
  connect(a, SIGNAL(triggered()), this, SLOT(close()));
  menu->addAction(a);

  traytop->setContextMenu(menu);

#if defined Q_OS_MACOS
     menu->setAsDockMenu();  //// only mac osx
#endif


  QWidget *panel = new QWidget(this);
  panel->setContentsMargins(0, 0, 0, 0);
  base_edit = new EditorKernel();
  base_edit->setContentsMargins(0, 0, 0, 0);
  gridLayout = new QGridLayout(panel);
  //// gridLayout->addWidget(Umargin, 0, 0, 1, 1);
  gridLayout->addWidget(base_edit, 1, 0, 1, 1);
  setCentralWidget(panel);
  statusbar = new QStatusBar(this);
  statusbar->setMaximumHeight(toolbarhight);
    statustxt = new QLabel("Application Loading...");
    statustxt->setFixedWidth(255);
    statustxt2 = new QLabel("");
    statustxt2->setFixedWidth(255);
  statusbar->addWidget(statustxt);
  statusbar->addWidget(statustxt2);
  statusbar->setObjectName(QString::fromUtf8("statusbar"));
  setStatusBar(statusbar);
  setupTextActions();
}
void OasiMain::setupTextActions() {
  QToolBar *tb = new QToolBar(this);
  tb->setWindowTitle(tr("Format Actions"));
  tb->setMaximumHeight(toolbarhight);
  addToolBar(tb);

  QMenu *menu = new QMenu(tr("F&ormat"), this);
  menuBar()->addMenu(menu);

  actionTextBold =
      new QAction(QIcon(rsrcPath + "/textbold.png"), tr("&Bold"), this);
  actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
  QFont bold;
  bold.setBold(true);
  actionTextBold->setFont(bold);
  connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
  tb->addAction(actionTextBold);
  menu->addAction(actionTextBold);
  actionTextBold->setCheckable(true);

  actionTextItalic =
      new QAction(QIcon(rsrcPath + "/textitalic.png"), tr("&Italic"), this);
  actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
  QFont italic;
  italic.setItalic(true);
  actionTextItalic->setFont(italic);
  connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
  tb->addAction(actionTextItalic);
  menu->addAction(actionTextItalic);
  actionTextItalic->setCheckable(true);

  actionTextUnderline =
      new QAction(QIcon(rsrcPath + "/textunder.png"), tr("&Underline"), this);
  actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
  QFont underline;
  underline.setUnderline(true);
  actionTextUnderline->setFont(underline);
  connect(actionTextUnderline, SIGNAL(triggered()), this,
          SLOT(textUnderline()));
  tb->addAction(actionTextUnderline);
  menu->addAction(actionTextUnderline);
  actionTextUnderline->setCheckable(true);

  menu->addSeparator();

  QActionGroup *grp = new QActionGroup(this);
  connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(textAlign(QAction *)));

  actionAlignLeft =
      new QAction(QIcon(rsrcPath + "/textleft.png"), tr("&Left"), grp);
  actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
  actionAlignLeft->setCheckable(true);
  actionAlignCenter =
      new QAction(QIcon(rsrcPath + "/textcenter.png"), tr("C&enter"), grp);
  actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
  actionAlignCenter->setCheckable(true);
  actionAlignRight =
      new QAction(QIcon(rsrcPath + "/textright.png"), tr("&Right"), grp);
  actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
  actionAlignRight->setCheckable(true);
  actionAlignJustify =
      new QAction(QIcon(rsrcPath + "/textjustify.png"), tr("&Justify"), grp);
  actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
  actionAlignJustify->setCheckable(true);

  tb->addActions(grp->actions());
  menu->addActions(grp->actions());

  menu->addSeparator();

  QPixmap pix(16, 16);
  pix.fill(Qt::black);
  actionTextColor = new QAction(pix, tr("&Color..."), this);
  connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
  tb->addAction(actionTextColor);
  menu->addAction(actionTextColor);

  tb = new QToolBar(this);
  tb->setMaximumHeight(toolbarhight);
  tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
  tb->setWindowTitle(tr("Format Actions"));
  addToolBarBreak(Qt::TopToolBarArea);
  addToolBar(tb);

  comboStyle = new QComboBox(tb);
  //// comboStyle->setMaximumHeight(toolbarhight);
  tb->addWidget(comboStyle);
  comboStyle->addItem("Standard");
  comboStyle->addItem("Bullet List (Disc)");
  comboStyle->addItem("Bullet List (Circle)");
  comboStyle->addItem("Bullet List (Square)");
  comboStyle->addItem("Ordered List (Decimal)");
  comboStyle->addItem("Ordered List (Alpha lower)");
  comboStyle->addItem("Ordered List (Alpha upper)");
  connect(comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

  comboFont = new QFontComboBox(tb);
  tb->addWidget(comboFont);
  connect(comboFont, SIGNAL(activated(const QString &)), this,
          SLOT(textFamily(const QString &)));

  comboSize = new QComboBox(tb);
  comboSize->setObjectName("comboSize");
  tb->addWidget(comboSize);
  comboSize->setEditable(true);

  QFontDatabase db;
  foreach (int size, db.standardSizes())
    comboSize->addItem(QString::number(size));

  connect(comboSize, SIGNAL(activated(const QString &)), this,
          SLOT(textSize(const QString &)));
  comboSize->setCurrentIndex(
      comboSize->findText(QString::number(QApplication::font().pointSize())));

  QToolBar *tbe = new QToolBar(this);
  tbe->setWindowTitle(tr("Edit Actions"));
  tbe->setMaximumHeight(toolbarhight);
  addToolBar(tbe);

  QMenu *menue = new QMenu(tr("&Edit"), this);
  menuBar()->addMenu(menue);

  QAction *a;
  a = actionUndo = new QAction(QIcon(rsrcPath + "/editundo.png"), tr("&Undo"), this);
  a->setShortcut(QKeySequence::Undo);
  tbe->addAction(a);
  menue->addAction(a);
  a = actionRedo = new QAction(QIcon(rsrcPath + "/editredo.png"), tr("&Redo"), this);
  a->setShortcut(QKeySequence::Redo);
  tbe->addAction(a);
  menue->addAction(a);
  menue->addSeparator();
  a = actionCut =
      new QAction(QIcon(rsrcPath + "/editcut.png"), tr("Cu&t"), this);
  a->setShortcut(QKeySequence::Cut);
  tbe->addAction(a);
  menue->addAction(a);
  a = actionCopy =
      new QAction(QIcon(rsrcPath + "/editcopy.png"), tr("&Copy"), this);
  a->setShortcut(QKeySequence::Copy);
  tbe->addAction(a);
  menue->addAction(a);
  a = actionPaste =
      new QAction(QIcon(rsrcPath + "/editpaste.png"), tr("&Paste"), this);
  a->setShortcut(QKeySequence::Paste);
  tbe->addAction(a);
  menue->addAction(a);
  actionPaste->setEnabled(false);




}



bool OasiMain::maybeSave() {
  if (!base_edit->document()->isModified())
    return true;
  if (fileName.startsWith(QLatin1String(":/")))
    return true;

  QMessageBox::StandardButton ret;
  ret = QMessageBox::warning(this, tr("Application"),
                             tr("The document has been modified.\n"
                                "Do you want to save your changes?"),
                             QMessageBox::Save | QMessageBox::Discard |
                                 QMessageBox::Cancel);
  if (ret == QMessageBox::Save)
    return fileSave();
  else if (ret == QMessageBox::Cancel)
    return false;
  return true;
}

void OasiMain::setCurrentFileName(const QString &fileName) {
  this->fileName = fileName;
  base_edit->document()->setModified(false);

  QString shownName;
  if (fileName.isEmpty())
    shownName = "untitled.txt";
  else
    shownName = QFileInfo(fileName).fileName();
  setWindowTitle(_CVERSION_ +
                 QString(" - %1 - OASIS Open Document").arg(shownName));
  setWindowModified(false);
  base_edit->modus_edit(true);
}

void OasiMain::fileNew() {
  if (maybeSave()) {
    base_edit->clear();
    setCurrentFileName(QString());
    base_edit->modus_edit(true);
  }
}

void OasiMain::fileOpen() {
  QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."), QString(setter.value("LastDir").toString()),QString("Document .rtf .odt .doc .docx .html .txt (*)"));

  ///// void QFileDialog::setHistory(const QStringList &paths)
  QFileDialog dialog(this);
  ///// dialog.setAttribute(Qt::WidgetAttribute,true);  //// Qt::WidgetAttribute
  ////dialog.setMimeTypeFilters(FileFilterHaving());
  ////// dialog.exec();
  ///
  if (!fn.isEmpty()) {
    QFileInfo fi(fn);
    setter.setValue("LastDir", fi.absolutePath() + "/");
    load(fn);
  }
}

bool OasiMain::fileSave() {
  if (fileName.isEmpty())
    return fileSaveAs();

  bool canodt = true;
  const QString ext = QFileInfo(fileName).completeSuffix().toLower();
  if (ext == "odt" && canodt) {
    QTextDocumentWriter writer(fileName);
    return writer.write(base_edit->document());
  } else {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly))
      return false;
    QString htmlchunk;
    DOC::self(this)->DecodeHtml(htmlchunk,base_edit->document());
    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("UTF-8"));
    ts << htmlchunk;
    base_edit->document()->setModified(false);
    file.close();
  }
  return true;
}

bool OasiMain::fileSaveAs() {

  QString support;
#if QT_VERSION >= 0x040500
  support = tr("ODF files (*.odt);;HTML-Files (*.htm *.html);;All Files (*)");
#else
  support = tr("HTML-Files (*.htm *.html);;All Files (*)");
#endif
  QString fn =
      QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), support);
  if (fn.isEmpty())
    return false;
  setCurrentFileName(fn);
  return fileSave();
}

void OasiMain::filePrint() {
#ifndef QT_NO_PRINTER
  QPrinter printer(QPrinter::HighResolution);
  QPrintDialog *dlg = new QPrintDialog(&printer, this);
  if (base_edit->textCursor().hasSelection())
    dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
  dlg->setWindowTitle(tr("Print Document"));
  if (dlg->exec() == QDialog::Accepted) {
    base_edit->print(&printer);
  }
  delete dlg;
#endif
}

void OasiMain::filePrintPreview() {
#ifndef QT_NO_PRINTER
  QPrinter printer(QPrinter::HighResolution);
  QPrintPreviewDialog preview(&printer, this);
  preview.setWindowFlags(Qt::Window);
  connect(&preview, SIGNAL(paintRequested(QPrinter *)),
          SLOT(printPreview(QPrinter *)));
  preview.exec();
#endif
}

void OasiMain::printPreview(QPrinter *printer) {
#ifndef QT_NO_PRINTER
  base_edit->print(printer);
#endif
}

void OasiMain::filePrintPdf() {
#ifndef QT_NO_PRINTER
  //! [0]
  QString fileName =
      QFileDialog::getSaveFileName(this, "Export PDF", QString(), "*.pdf");
  if (!fileName.isEmpty()) {
    if (QFileInfo(fileName).suffix().isEmpty())
      fileName.append(".pdf");
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    base_edit->document()->print(&printer);
  }
//! [0]
#endif
}

void OasiMain::textBold() {
  QTextCharFormat fmt;
  fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
  mergeFormatOnWordOrSelection(fmt);
}

void OasiMain::textUnderline() {
  QTextCharFormat fmt;
  fmt.setFontUnderline(actionTextUnderline->isChecked());
  mergeFormatOnWordOrSelection(fmt);
}

void OasiMain::textItalic() {
  QTextCharFormat fmt;
  fmt.setFontItalic(actionTextItalic->isChecked());
  mergeFormatOnWordOrSelection(fmt);
}

void OasiMain::textFamily(const QString &f) {
  QTextCharFormat fmt;
  fmt.setFontFamily(f);
  mergeFormatOnWordOrSelection(fmt);
}

void OasiMain::textSize(const QString &p) {
  QTextCharFormat fmt;
  fmt.setFontPointSize(p.toFloat());
  mergeFormatOnWordOrSelection(fmt);
}

void OasiMain::textStyle(int styleIndex) {
  QTextCursor cursor = base_edit->textCursor();

  if (styleIndex != 0) {
    QTextListFormat::Style style = QTextListFormat::ListDisc;

    switch (styleIndex) {
    default:
    case 1:
      style = QTextListFormat::ListDisc;
      break;
    case 2:
      style = QTextListFormat::ListCircle;
      break;
    case 3:
      style = QTextListFormat::ListSquare;
      break;
    case 4:
      style = QTextListFormat::ListDecimal;
      break;
    case 5:
      style = QTextListFormat::ListLowerAlpha;
      break;
    case 6:
      style = QTextListFormat::ListUpperAlpha;
      break;
    }

    cursor.beginEditBlock();

    QTextBlockFormat blockFmt = cursor.blockFormat();

    QTextListFormat listFmt;

    if (cursor.currentList()) {
      listFmt = cursor.currentList()->format();
    } else {
      listFmt.setIndent(blockFmt.indent() + 1);
      blockFmt.setIndent(0);
      cursor.setBlockFormat(blockFmt);
    }

    listFmt.setStyle(style);

    cursor.createList(listFmt);

    cursor.endEditBlock();
  } else {
    // ####
    QTextBlockFormat bfmt;
    bfmt.setObjectIndex(-1);
    cursor.mergeBlockFormat(bfmt);
  }
}

void OasiMain::textColor() {
  QColor col = QColorDialog::getColor(base_edit->textColor(), this);
  if (!col.isValid())
    return;
  QTextCharFormat fmt;
  fmt.setForeground(col);
  mergeFormatOnWordOrSelection(fmt);
  colorChanged(col);
}

void OasiMain::textAlign(QAction *a) {
  if (a == actionAlignLeft)
    base_edit->setAlignment(Qt::AlignLeft);
  else if (a == actionAlignCenter)
    base_edit->setAlignment(Qt::AlignHCenter);
  else if (a == actionAlignRight)
    base_edit->setAlignment(Qt::AlignRight);
  else if (a == actionAlignJustify)
    base_edit->setAlignment(Qt::AlignJustify);
}

void OasiMain::currentCharFormatChanged(const QTextCharFormat &format) {
  fontChanged(format.font());
  colorChanged(format.foreground().color());
}

void OasiMain::cursorPositionChanged() {
  alignmentChanged(base_edit->alignment());
}

void OasiMain::clipboardDataChanged() {
  actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

void OasiMain::DocumentChanged() {
bool itext = !QApplication::clipboard()->text().isEmpty();
bool isimage = !QApplication::clipboard()->image().isNull();
if (itext) {
    actionPaste->setEnabled(true);
}
if (isimage) {
    ///// actionPaste->setEnabled(true);
}

actionSave->setEnabled(!base_edit->document()->isModified());
}



void OasiMain::about() {
  QMessageBox::about(
      this, tr("About"),
      tr("This example demonstrates different file format..."));
}

void OasiMain::mergeFormatOnWordOrSelection(const QTextCharFormat &format) {
  QTextCursor cursor = base_edit->textCursor();
  if (!cursor.hasSelection())
    cursor.select(QTextCursor::WordUnderCursor);
  cursor.mergeCharFormat(format);
  base_edit->mergeCurrentCharFormat(format);
}


void OasiMain::fontChanged(const QFont &f) {
  comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
  comboSize->setCurrentIndex(
      comboSize->findText(QString::number(f.pointSize())));
  actionTextBold->setChecked(f.bold());
  actionTextItalic->setChecked(f.italic());
  actionTextUnderline->setChecked(f.underline());
}

void OasiMain::colorChanged(const QColor &c) {
  QPixmap pix(16, 16);
  pix.fill(c);
  actionTextColor->setIcon(pix);
}

void OasiMain::alignmentChanged(Qt::Alignment a) {
  if (a & Qt::AlignLeft) {
    actionAlignLeft->setChecked(true);
  } else if (a & Qt::AlignHCenter) {
    actionAlignCenter->setChecked(true);
  } else if (a & Qt::AlignRight) {
    actionAlignRight->setChecked(true);
  } else if (a & Qt::AlignJustify) {
    actionAlignJustify->setChecked(true);
  }
}



void OasiMain::setTextMsg( const QString txt ) {
   //// QLabel *statustxt2, *statustxt;
   statustxt2->setText(txt);
}

void OasiMain::setTextTray( const QString txt ) {
   //// QLabel *statustxt2, *statustxt;
   if (!traytop->isVisible()) {
      traytop->setVisible(true);
   }
   traytop->showMessage(QString("Document Msg."),txt,QSystemTrayIcon::Information,(5*1000));
   statustxt->setText(txt);
}


