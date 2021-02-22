
#include <QtGui>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QComboBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include "CollectWindow.h"
#include "FeatureWidget.h"
#include "Dialogs/FindDialog.h"
#include "Dialogs/InsertRCDialog.h"
#include "Dialogs/NewFeatureDialog.h"
//#include "Dialogs/SortDialog.h"
#include "Commands.h"
#include "FeatureWidget.h"
//#include "FunctionListDialog.h"
//#include "Calculator.h"
//#include "Graphic.h"
//#include "PictureGraphic.h"
//#include "GraphicPropertiesDialog.h"
//#include "SortDialog.h"
#include <iostream> //NOTE: for debugging

CollectWindow::CollectWindow(QWidget *parent, Qt::WindowFlags flags)
         : QMainWindow(parent, flags)
{

   findDialog = nullptr;
//   functionListDialog = 0;
//   table = new FeatureWidget(this);
   feaWidgets = new QTabWidget(this);
   setCentralWidget(feaWidgets);
//   setWindowState(Qt::WindowMaximized);
   setWindowIcon(QIcon(":/CustomViewer/applicationIcon.png"));

//   connect(table, SIGNAL(modified()), this, SLOT(documentModified()));
//   connect(table, SIGNAL(userChangedItemDataSignal(int, int, QString&, QString&)),
//               this, SLOT(userChangedItemDataSlot(int, int, QString&, QString&)));
//   connect(table, SIGNAL(itemSelectionChanged()),
//           this, SLOT(updateActions()));

//   table -> setContextMenuPolicy(Qt::ActionsContextMenu);

   creatActions();
   creatMenus();
   creatToolBars();
   creatStatusBar();
   creatContextMenu();

   setCurrentFile("");
   addFeature();
   setWindowModified(false);
}

CollectWindow::~CollectWindow()
{

}

void CollectWindow::creatActions()
{
//   undoAction = table ->getUndoStack()->createUndoAction(this);
//   undoAction -> setShortcut(tr("Ctrl+Z"));
//   undoAction -> setIcon(QIcon(":/CustomViewer/undo.png"));
//   redoAction = table -> getUndoStack() -> createRedoAction(this);
//   redoAction -> setShortcut(tr("Ctrl+Y"));
//   redoAction -> setIcon(QIcon(":/CustomViewer/redo.png"));

   newCollectAction = new QAction(tr("&New"), this);
   newCollectAction -> setShortcut(tr("Ctrl+N"));
   newCollectAction -> setIcon(QIcon(":/GPjsonEditor/new.png"));
   connect(newCollectAction, SIGNAL(triggered()), this, SLOT(newFile()));

   openCollectAction = new QAction(tr("&Open"), this);
   openCollectAction -> setShortcut(tr("Ctrl+O"));
   openCollectAction -> setIcon(QIcon(":/GPjsonEditor/open.png"));
   connect(openCollectAction, SIGNAL(triggered()), this, SLOT(open()));

   addFeatureAction = new QAction(tr("&Add"), this);
   addFeatureAction -> setShortcut(tr("Ctrl+A"));
   addFeatureAction -> setIcon(QIcon(":/GPjsonEditor/add.png"));
   connect(addFeatureAction, SIGNAL(triggered()), this, SLOT(addFeature()));

   exitAction = new QAction(tr("&Exit"), this);
   exitAction -> setShortcut(tr("Ctrl+Q"));
   exitAction -> setIcon(QIcon(":/GPjsonEditor/exit.png"));
   connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
//   QAction *openCollectAction;
//   QAction *addFeatureAction;
//   QAction *removeFeatureAction;
//   QAction *closeCollectAction;
//   QAction *saveAction;        // 保存FeatureCollect
//   QAction *saveAsAction;      // 另存FeatureCollect
//   QAction *saveFeatureToFileAction;    // 保存当前的Feature 到独立文件


   
//   saveAction = new QAction(tr("&Save"), this);
//   saveAction -> setShortcut(tr("Ctrl+S"));
//   saveAction -> setIcon(QIcon(":/CustomViewer/save.png"));
//   connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
   
//   saveAsAction = new QAction(tr("Save &As"), this);
//   saveAsAction -> setShortcut(tr("Ctrl+Shift+S"));
//   saveAsAction -> setIcon(QIcon(":/CustomViewer/saveAs.png"));
//   connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
   

      
//   copyAction = new QAction(tr("&Copy"), this);
//   copyAction -> setShortcut(tr("Ctrl+C"));
//   copyAction -> setIcon(QIcon(":/CustomViewer/copy.png"));
//   connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
   
//   cutAction = new QAction(tr("C&ut"), this);
//   cutAction -> setShortcut(tr("Ctrl+X"));
//   cutAction -> setIcon(QIcon(":/CustomViewer/cut.png"));
//   connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
   
//   pasteAction = new QAction(tr("&Paste"), this);
//   pasteAction -> setShortcut(tr("Ctrl+V"));
//   pasteAction -> setIcon(QIcon(":/CustomViewer/paste.png"));
//   connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
   
//   findAction = new QAction(tr("&Find..."), this);
//   findAction -> setShortcut(tr("Ctrl+F"));
//   findAction -> setIcon(QIcon(":/CustomViewer/find.png"));
//   connect(findAction, SIGNAL(triggered()), this, SLOT(find()));
   
//   findNextAction = new QAction(tr("Find &Next"), this);
//   findNextAction -> setShortcut(tr("F3"));
//   connect(findNextAction, SIGNAL(triggered()), this, SLOT(findNext()));
   
//   findPreviousAction = new QAction(tr("Find &Previous"), this);
//   findPreviousAction -> setShortcut(tr("Shift+F3"));
//   connect(findPreviousAction, SIGNAL(triggered()), this, SLOT(findPrevious()));
   
//   deleteContentsAction = new QAction(tr("&Delete"), this);
//   deleteContentsAction -> setShortcut(tr("Del"));
//   connect(deleteContentsAction, SIGNAL(triggered()), this, SLOT(deleteContents()));
   
//   deleteEntireRowsAction = new QAction(tr("Delete Entire &Row(s)"), this);
//   deleteEntireRowsAction -> setShortcut(tr("Ctrl+Del"));
//   connect(deleteEntireRowsAction, SIGNAL(triggered()), this, SLOT(deleteEntireRows()));
   
//   deleteEntireColumnsAction = new QAction(tr("Delete Entire C&olumn(s)"), this);
//   deleteEntireColumnsAction -> setShortcut(tr("Ctrl+Shift+Del"));
//   connect(deleteEntireColumnsAction, SIGNAL(triggered()), this, SLOT(deleteEntireColumns()));
   
//   selectCurRowAction = new QAction(tr("Select Current Ro&w"), this);
//   connect(selectCurRowAction, SIGNAL(triggered()), table, SLOT(selectCurrentRow()));
   
//   selectCurColAction = new QAction(tr("Select Current Co&lumn"), this);
//   connect(selectCurColAction, SIGNAL(triggered()), table, SLOT(selectCurrentColumn()));
   
//   selectAllAction = new QAction(tr("Select &All"), this);
//   selectAllAction -> setShortcut(tr("Ctrl+A"));
//   connect(selectAllAction, SIGNAL(triggered()), table, SLOT(selectAll()));
   
//   insertRowsAction = new QAction(tr("Insert &Rows"), this);
//   insertRowsAction -> setShortcut(tr("Ctrl+Shift+R"));
//   connect(insertRowsAction, SIGNAL(triggered()), this, SLOT(insertRows()));
   
//   insertColumnsAction = new QAction(tr("Insert &Columns"), this);
//   insertColumnsAction -> setShortcut(tr("Ctrl+Shift+C"));
//   connect(insertColumnsAction, SIGNAL(triggered()), this, SLOT(insertColumns()));
     
//   textLineFormatAction = new QAction(tr("&Text Line (default)"), this);
//   connect(textLineFormatAction, SIGNAL(triggered()), this, SLOT(textLineFormat()));
   
//   dateFormatAction = new QAction(tr("&Date"), this);
//   connect(dateFormatAction, SIGNAL(triggered()), this, SLOT(dateFormat()));
   
//   timeFormatAction = new QAction(tr("T&ime"), this);
//   connect(timeFormatAction, SIGNAL(triggered()), this, SLOT(timeFormat()));
   
//   integerFormatAction = new QAction(tr("I&nteger"), this);
//   connect(integerFormatAction, SIGNAL(triggered()), this, SLOT(integerFormat()));
   
//   decimalFormatAction = new QAction(tr("D&ecimal"), this);
//   connect(decimalFormatAction, SIGNAL(triggered()), this, SLOT(decimalFormat()));
   
//   fontAction = new QAction(tr("&Font.."), this);
//   fontAction -> setIcon(QIcon(":/CustomViewer/font.png"));
//   connect(fontAction, SIGNAL(triggered()), this, SLOT(font()));
   
//   alignLeftAction = new QAction(tr("Align &Left"), this);
//   alignLeftAction -> setCheckable(true);
//   alignLeftAction -> setIcon(QIcon(":/CustomViewer/alignLeft.png"));
//   connect(alignLeftAction, SIGNAL(toggled(bool)), this, SLOT(align(bool)));
   
//   alignCenterAction = new QAction(tr("Align &Center"), this);
//   alignCenterAction -> setCheckable(true);
//   alignCenterAction -> setIcon(QIcon(":/CustomViewer/alignCenter.png"));
//   connect(alignCenterAction, SIGNAL(toggled(bool)), this, SLOT(align(bool)));
   
//   alignRightAction = new QAction(tr("Align &Right"), this);
//   alignRightAction -> setCheckable(true);
//   alignRightAction -> setIcon(QIcon(":/CustomViewer/alignRight.png"));
//   connect(alignRightAction, SIGNAL(toggled(bool)), this, SLOT(align(bool)));
   
//   QActionGroup *alignGroup = new QActionGroup(this);
//   alignGroup -> setExclusive(true);
//   alignGroup -> addAction(alignLeftAction);
//   alignGroup -> addAction(alignCenterAction);
//   alignGroup -> addAction(alignRightAction);
     
//   sortAction = new QAction(tr("&Sort"), this);
//   connect(sortAction, SIGNAL(triggered()), this, SLOT(sort()));
   
}

void CollectWindow::creatMenus()
{
   fileMenu = menuBar() -> addMenu(tr("&File"));
   fileMenu -> addAction(newCollectAction);
   fileMenu -> addAction(openCollectAction);
   fileMenu -> addSeparator();
   fileMenu -> addAction(addFeatureAction);
//   fileMenu -> addAction(saveAction);
//   fileMenu -> addAction(saveAsAction);
   fileMenu -> addSeparator();
   fileMenu -> addAction(exitAction);

   editMenu = menuBar() -> addMenu(tr("&Edit"));
//   editMenu -> addAction(undoAction);
//   editMenu -> addAction(redoAction);
//   editMenu -> addSeparator();
//   editMenu -> addAction(copyAction);
//   editMenu -> addAction(cutAction);
//   editMenu -> addAction(pasteAction);
//   editMenu -> addSeparator();
//   editMenu -> addAction(findAction);
//   editMenu -> addAction(findNextAction);
//   editMenu -> addAction(findPreviousAction);
//   editMenu -> addSeparator();
//   editMenu -> addAction(deleteContentsAction);
//   editMenu -> addAction(deleteEntireRowsAction);
//   editMenu -> addAction(deleteEntireColumnsAction);
//   editMenu -> addSeparator();
//   editMenu -> addAction(selectAllAction);
//   editMenu -> addAction(selectCurRowAction);
//   editMenu -> addAction(selectCurColAction);
   
//   insertMenu = menuBar() -> addMenu(tr("&Insert"));
//   insertMenu -> addAction(insertRowsAction);
//   insertMenu -> addAction(insertColumnsAction);
     
//   toolsMenu = menuBar() -> addMenu(tr("&Tools"));
//   toolsMenu -> addAction(sortAction);
   
   optionsMenu = menuBar() -> addMenu(tr("&Options"));
//   QMenu *dataFormat = optionsMenu-> addMenu(tr("&Data Format"));
//   dataFormat -> addAction(textLineFormatAction);
//   dataFormat -> addAction(timeFormatAction);
//   dataFormat -> addAction(dateFormatAction);
//   dataFormat -> addAction(integerFormatAction);
//   dataFormat -> addAction(decimalFormatAction);
//   optionsMenu -> addAction(fontAction);
   optionsMenu -> addSeparator();
//   optionsMenu -> addAction(showGridAction);
//   formatMenu -> addAction(alignLeftAction);
//   formatMenu -> addAction(alignCenterAction);
//   formatMenu -> addAction(alignRightAction);
   
   helpMenu = menuBar() -> addMenu(tr("&Help"));

}

void CollectWindow::creatToolBars()
{
   mainToolBar = addToolBar(tr("Main Toolbar"));

   mainToolBar -> addSeparator();
//   mainToolBar -> addAction(saveAction);
//   mainToolBar -> addAction(saveAsAction);
   mainToolBar -> addSeparator();
//   mainToolBar -> addAction(undoAction);
//   mainToolBar -> addAction(redoAction);
//   mainToolBar -> addSeparator();
//   mainToolBar -> addAction(copyAction);
//   mainToolBar -> addAction(cutAction);
//   mainToolBar -> addAction(pasteAction);
//   mainToolBar -> addSeparator();
//   mainToolBar -> addAction(alignLeftAction);
//   mainToolBar -> addAction(alignCenterAction);
//   mainToolBar -> addAction(alignRightAction);
   mainToolBar -> addSeparator();
//   mainToolBar -> addAction(fontAction);
}

void CollectWindow::creatStatusBar()
{
   currentCellDataLabel = new QLabel("No cell is selected.");
   currentCellDataLabel -> setIndent(5);
   statusBar() -> addWidget(currentCellDataLabel, 1);
//   connect(table, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(updateStatusBar()));
}

void CollectWindow::creatContextMenu()
{
   //FIXME: any better ways???
   QAction *separator1 = new QAction(this);
   separator1 -> setSeparator(true);
   
   QAction *separator2 = new QAction(this);
   separator2 -> setSeparator(true);
   
   QAction *separator3 = new QAction(this);
   separator3 -> setSeparator(true);
   
   QAction *separator4 = new QAction(this);
   separator4 -> setSeparator(true);
   
//   QAction *cellFormat = new QAction(tr("Cell Format"), this);
//   cellFormat -> setMenu(optionsMenu);

//   table -> addAction(cellFormat);
//   table -> addAction(separator1);
////   table -> addAction(undoAction);
////   table -> addAction(redoAction);
//   table -> addAction(separator2);
//   table -> addAction(copyAction);
//   table -> addAction(cutAction);
//   table -> addAction(pasteAction);
//   table -> addAction(separator3);
//   table -> addAction(deleteContentsAction);
//   table -> addAction(deleteEntireRowsAction);
//   table -> addAction(deleteEntireColumnsAction);
//   table -> addAction(separator4);
//   table -> addAction(showGridAction);
}

void CollectWindow::updateStatusBar()
{
   //TODO: it should show the SUM
   QString shownText = "Sum = ";
//   if (!Calculator::isFormula(table -> getCurrentItemText(Qt::EditRole)))
//      shownText += "0";
//   else
//      shownText += table -> getCurrentItemText();
   currentCellDataLabel -> setText(shownText);
}

bool CollectWindow::askToSaveChanges()
{
   if (isWindowModified())
   {
      int answer = QMessageBox::warning(this, "Save",
                                        "This document has been modified.\n"
                                        "Do you want to save it?",
                                        QMessageBox::Yes | QMessageBox::Default,
                                        QMessageBox::No,
                                        QMessageBox::Cancel | QMessageBox::Escape);
      if (answer == QMessageBox::Cancel)
         return false;
      else if (answer == QMessageBox::Yes)
         return save();
   }
   return true;
}

bool CollectWindow::newFile()
{
    if (!askToSaveChanges())
        return false;

    setCurrentFile("");
    feaCollect.clear();
    QSharedPointer<GPJson::Feature> fea(new GPJson::Feature);
    feaCollect.list.append(fea);


    return true;
}

bool CollectWindow::addFeature()
{
    NewFeatureDialog dialog(this);
    if (dialog.exec())
    {
       if(dialog.radioButtonSheet->isChecked() || dialog.radioButtonMatrix->isChecked())
       {
           int rowCount = dialog.rowsSpinBox -> value();
           int columnCount = dialog.columnsSpinBox -> value();
//           fea->dataset.source.insert(0,rowCount,QJsonArray());
//           newSheet(rowCount, columnCount,dialog.radioButtonSheet->isChecked() ? GPJson::Sheet : GPJson::Matrix );

       }
       if(dialog.radioButtonBinary->isChecked() || dialog.radioButtonURI->isChecked())
       {
//           newTextEidt(dialog.radioButtonBinary->isChecked() ? GPJson::Binary : GPJson::URI);
       }


       setWindowModified(true);
       return true;
    }

    FeatureWidget *childWidget = new FeatureWidget;
//    feaCollect.list.append(childWidget->feature());
    feaWidgets->addTab(childWidget,"sheet");
    return false;
}


bool CollectWindow::open(QString fileName)
{
//    setCurrentFile( table->importASCII());
    return true;
}

bool CollectWindow::save()
{
   if (currentFile.isEmpty())
      return saveAs();
   else
      return saveFile(currentFile);
}

bool CollectWindow::saveAs()
{
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), ".",
                                                   "GPJson files (*.gpjson *.GPJSON );;"
                                                    "Text files (*.TXT *.txt);;"
                                                     "Data files (*.DAT *.dat)");

   if (!fileName.isEmpty())
   {
//      table->exportASCII(fileName);
      setCurrentFile(fileName);
      setWindowModified(false);
      return true;
   }
   return false;
}

bool CollectWindow::saveFile(const QString &fileName)
{
//   if(table->exportASCII(fileName))
   {
      setCurrentFile(fileName);
      setWindowModified(false);
      return true;
   }
   return false;
}

void CollectWindow::setCurrentFile(const QString &fullFileName)
{
   QString shownName = "Untitled";
   if (!fullFileName.isEmpty())
      shownName = QFileInfo(fullFileName).fileName();
   setWindowTitle(shownName + "[*] - The dataset of GPJson");
   currentFile = fullFileName;
}

void CollectWindow::documentModified()
{
   setWindowModified(true);
   updateStatusBar();
   updateActions();
}

void CollectWindow::userChangedItemDataSlot(int row, int column,
                                         QVariant &oldData, QVariant &newData)
{
//   table -> getUndoStack() -> push(new DataChanged(table, row, column, oldData, newData));
}

void CollectWindow::updateActions()
{
//   if (isWindowModified())
//      saveAction -> setEnabled(true);
//   else
//      saveAction -> setEnabled(false);
   
//   QItemSelectionRange selectedRange = table -> getSelectedRange();
   
//   if (selectedRange.height() > 1 || selectedRange.width() > 1)
//   {
//      alignLeftAction -> setChecked(false);
//      alignCenterAction -> setChecked(false);
//      alignRightAction -> setChecked(false);
//   }
//   else
//   {
//      int alignment = table -> getItemAlignment(selectedRange.topRow(),
//                                                selectedRange.leftColumn());
//      if (alignment == (Qt::AlignLeft | Qt::AlignVCenter))
//         alignLeftAction -> setChecked(true);
//      else if (alignment == Qt::AlignCenter)
//         alignCenterAction -> setChecked(true);
//      else if (alignment == (Qt::AlignRight | Qt::AlignVCenter))
//         alignRightAction -> setChecked(true);
//   }
}

void CollectWindow::copy()
{
//   QApplication::clipboard()->setText(table->getItemsInByteArray(table->getSelectedRange()), QClipboard::Selection);
//   QMimeData *mimeData = new QMimeData;
//   table -> copy(*mimeData);
//   QApplication::clipboard()->setMimeData(mimeData);
}

void CollectWindow::cut()
{
//   table -> getUndoStack() -> push(new CutCommand(table));
}

void CollectWindow::paste()
{
//   table -> getUndoStack() -> push(new PasteCommand(table));
}

void CollectWindow::deleteContents()
{
//   table -> getUndoStack() -> push(new DeleteContents(table));
}

void CollectWindow::deleteEntireRows()
{
//   QItemSelectionRange range = table -> getSelectedRange();
//   int position = range.top();
//   int amount = range.height();
//   table -> getUndoStack() -> push(new DeleteEntireRows(table, position, amount));
}

void CollectWindow::deleteEntireColumns()
{
//   QItemSelectionRange range = table -> getSelectedRange();
//   int position = range.top();
//   int amount = range.height();
//   table -> getUndoStack() -> push(new DeleteEntireColumns(table, position, amount));
}

void CollectWindow::insertRows()
{
//   InsertRCDialog dialog(InsertRCDialog::InsertRows, this);
//   if (dialog.exec())
//   {
//      int amount = dialog.amountSpinBox -> value();
//      int position = dialog.afterRadioButton ->
//                     isChecked() ? FeatureWidget::InsertAfter : FeatureWidget::InsertBefore;
//      table -> getUndoStack() -> push(new InsertRowsCommand(table, position, amount));
//   }
}

void CollectWindow::insertColumns()
{
//   InsertRCDialog dialog(InsertRCDialog::InsertColumns, this);
//   if (dialog.exec())
//   {
//      int amount = dialog.amountSpinBox -> value();
//      int position = dialog.afterRadioButton ->
//                     isChecked() ? FeatureWidget::InsertAfter : FeatureWidget::InsertBefore;
//      table -> getUndoStack() -> push(new InsertColumnsCommand(table, position, amount));
//   }
}

void CollectWindow::textLineFormat()
{
//   table -> getUndoStack() -> push(new ChangeCellsEditor(table, table->getSelectedRange(), FeatureWidget::TextLineEditor));
}

void CollectWindow::dateFormat()
{
//   table -> getUndoStack() -> push(new ChangeCellsEditor(table,
//                                   table->getSelectedRange(), Table::DateEditor));
}

void CollectWindow::timeFormat()
{
//   table -> getUndoStack() -> push(new ChangeCellsEditor(table,
//                                   table->getSelectedRange(), Table::TimeEditor));
}

void CollectWindow::integerFormat()
{
//   table -> getUndoStack() -> push(new ChangeCellsEditor(table,
//                                   table->getSelectedRange(), Table::IntegerEditor));
}

void CollectWindow::decimalFormat()
{
//   table -> getUndoStack() -> push(new ChangeCellsEditor(table,
//                                   table->getSelectedRange(), Table::DecimalEditor));
}

void CollectWindow::font()
{
//   bool ok;
//   QFont newFont = QFontDialog::getFont(&ok, table-> getItemFont(), this);
//   if (ok)
//      table -> getUndoStack() -> push(new ChangeCellsFont(table, newFont));
}

void CollectWindow::align(bool checked)
{
   if (!checked)
      return;
   
//   int alignment = Qt::AlignLeft;
//   if (alignCenterAction -> isChecked())
//      alignment = Qt::AlignHCenter;
//   else if (alignRightAction -> isChecked())
//      alignment = Qt::AlignRight;
//   alignment |= Qt::AlignVCenter;
      
//   QTableWidgetSelectionRange range = table -> getSelectedRange();
//   for (int i=0; i<range.rowCount(); i++)
//      for (int j=0; j<range.columnCount(); j++)
//         if (table -> getItemAlignment(i+range.topRow(), j+range.leftColumn()) != alignment)
//         {
//            table -> getUndoStack() -> push(new ChangeCellsAlignment(table, range, alignment));
//            return;
//         }
}

void CollectWindow::find()
{
   if (!findDialog)
   {
      findDialog = new FindDialog(this);
      connect(findDialog, SIGNAL(findNextSignal()), this, SLOT(findNext()));
      connect(findDialog, SIGNAL(findPreviousSignal()), this, SLOT(findPrevious()));
   }
   findDialog -> exec();
}

void CollectWindow::findNext()
{
//   if (!findDialog)
//      find();
//   else
//   {
//      QString text = findDialog->text();
//      table -> findNext(text, findDialog->caseSensitivity());
//   }
}

void CollectWindow::findPrevious()
{
//   if (!findDialog)
//      find();
//   else
//   {
//      QString text = findDialog->text();
//      table -> findPrevious(text, findDialog->caseSensitivity());
//   }
}

void CollectWindow::sort()
{
//   QItemSelectionRange range = table -> getSelectedRange();
//   if (range.height() <= 1 && range.width() <= 1)
//   {
//      QMessageBox::warning(this, tr("Nothing to sort"),
//            tr("You should select at least 2 rows/columns to sort."));
//      return;
//   }
//   SortDialog dialog(range, this);
//   if (dialog.exec())
//   {
//      table -> getUndoStack() -> push(new SortCommand(table, dialog.getTableSort()));
//   }
}

void CollectWindow::closeEvent(QCloseEvent *event)
{
   if (askToSaveChanges())
      event -> accept();
   else
      event -> ignore();
}


