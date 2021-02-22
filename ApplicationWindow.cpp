/****************************************************************************
**
** Copyright (C) Eageo Information & Exploration Technology Co.,Ltd.,
**
** Use of this file is limited according to the terms specified by
** Eageo Exploration & Information Technology Co.,Ltd.  Details of
** those terms are listed in licence.txt included as part of the distribution
** package ** of this file. This file may not be distributed without including
** the licence.txt file.
**
** Contact aben.lee@foxmail.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "ApplicationWindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileInfo>
#include <QFileDialog>
#include <QMenuBar>
#include <QToolBar>
#include <QUndoStack>

ApplicationWindow::ApplicationWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    collectW = new CollectWidget(this);
    setCentralWidget(collectW);
//    setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(":/CustomViewer/applicationIcon.png"));

//    connect(collectW, SIGNAL(modified()), this, SLOT(documentModified()));
 //   connect(collectW, SIGNAL(userChangedItemDataSignal(int, int, QString&, QString&)),
 //               this, SLOT(userChangedItemDataSlot(int, int, QString&, QString&)));
 //   connect(collectW, SIGNAL(itemSelectionChanged()),
 //           this, SLOT(updateActions()));

 //   collectW -> setContextMenuPolicy(Qt::ActionsContextMenu);

    creatActions();
    creatMenus();
    creatToolBars();
//    creatStatusBar();
//    creatContextMenu();

    setCurrentFile("");
    setWindowModified(false);
}

ApplicationWindow::~ApplicationWindow()
{
    delete collectW;
}

void ApplicationWindow::documentModified()
{
    setWindowModified(true);
    updateStatusBar();
    updateActions();
}

void ApplicationWindow::updateStatusBar()
{
    //TODO: it should show the SUM
    QString shownText = "Sum = ";
 //   if (!Calculator::isFormula(table -> getCurrentItemText(Qt::EditRole)))
 //      shownText += "0";
 //   else
 //      shownText += table -> getCurrentItemText();
    //    currentCellDataLabel -> setText(shownText);
}

void ApplicationWindow::updateActions()
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

void ApplicationWindow::creatActions()
{
    newCollectAction = new QAction(tr("&New"), this);
    newCollectAction -> setShortcut(tr("Ctrl+N"));
    newCollectAction -> setIcon(QIcon(":/GPjsonEditor/new.png"));
    connect(newCollectAction, SIGNAL(triggered()), collectW, SLOT(newFile()));

    openCollectAction = new QAction(tr("&Open"), this);
    openCollectAction -> setShortcut(tr("Ctrl+O"));
    openCollectAction -> setIcon(QIcon(":/GPjsonEditor/open.png"));
    connect(openCollectAction, SIGNAL(triggered()), collectW, SLOT(open()));

    closeCollectAction = new QAction(tr("&Close"), this);
//    closeCollectAction -> setShortcut(tr("Ctrl+O"));
    closeCollectAction -> setIcon(QIcon(":/GPjsonEditor/close.png"));
    connect(closeCollectAction, SIGNAL(triggered()), collectW, SLOT(close()));

    addFeatureAction = new QAction(tr("&Add"), this);
    addFeatureAction -> setShortcut(tr("Ctrl+A"));
    addFeatureAction -> setIcon(QIcon(":/GPjsonEditor/add.png"));
    connect(addFeatureAction, SIGNAL(triggered()), collectW, SLOT(addFeature()));

    removeFeatureAction = new QAction(tr("&Remove"), this);
    removeFeatureAction -> setShortcut(tr("Ctrl+R"));
    removeFeatureAction -> setIcon(QIcon(":/GPjsonEditor/add.png"));
    connect(removeFeatureAction, SIGNAL(triggered()), collectW, SLOT(removeFeature()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction -> setShortcut(tr("Ctrl+S"));
    saveAction -> setIcon(QIcon(":/GPjsonEditor/save.png"));
    connect(saveAction, SIGNAL(triggered()), collectW, SLOT(save()));

    saveAsAction = new QAction(tr("&Save As"), this);
    saveAsAction -> setShortcut(tr("Ctrl+A+S"));
    saveAsAction -> setIcon(QIcon(":/GPjsonEditor/saveAs.png"));
    connect(saveAsAction, SIGNAL(triggered()), collectW, SLOT(saveAs()));

    // 保存当前的Feature 到独立文件
    saveFeatureAction = new QAction(tr("&Save Feature"), this);
    saveFeatureAction -> setShortcut(tr("Ctrl+A+S"));
    saveFeatureAction -> setIcon(QIcon(":/GPjsonEditor/saveAs.png"));
    connect(saveFeatureAction, SIGNAL(triggered()), collectW, SLOT(saveFeature()));

    printAction = new QAction(tr("&Print"), this);
    printAction -> setShortcut(tr("Ctrl+P"));
    printAction -> setIcon(QIcon(":/GPjsonEditor/printer.png"));
    connect(printAction, SIGNAL(triggered()), collectW, SLOT(print()));

    printPDFAction = new QAction(tr("&Print PDF"), this);
//    printPDFAction -> setShortcut(tr("Ctrl+P+D"));
    printPDFAction -> setIcon(QIcon(":/GPjsonEditor/printview.png"));
    connect(printPDFAction, SIGNAL(triggered()), collectW, SLOT(printPDF()));

    exitAction = new QAction(tr("&Exit"), this);
    exitAction -> setShortcut(tr("Ctrl+Q"));
    exitAction -> setIcon(QIcon(":/GPjsonEditor/exit.png"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    undoAction = collectW -> getUndoStack() -> createUndoAction(this);
    undoAction -> setShortcut(tr("Ctrl+Z"));
    undoAction -> setIcon(QIcon(":/images/undo.png"));
    redoAction = collectW -> getUndoStack() -> createRedoAction(this);
    redoAction -> setShortcut(tr("Ctrl+Y"));
    redoAction -> setIcon(QIcon(":/images/redo.png"));


    copyAction = new QAction(tr("&Copy"), this);
    copyAction -> setShortcut(tr("Ctrl+C"));
    copyAction -> setIcon(QIcon(":/images/copy.png"));
    connect(copyAction, SIGNAL(triggered()), collectW, SLOT(copy()));

    cutAction = new QAction(tr("C&ut"), this);
    cutAction -> setShortcut(tr("Ctrl+X"));
    cutAction -> setIcon(QIcon(":/images/cut.png"));
    connect(cutAction, SIGNAL(triggered()), collectW, SLOT(cut()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction -> setShortcut(tr("Ctrl+V"));
    pasteAction -> setIcon(QIcon(":/images/paste.png"));
    connect(pasteAction, SIGNAL(triggered()), collectW, SLOT(paste()));

    findAction = new QAction(tr("&Find..."), this);
    findAction -> setShortcut(tr("Ctrl+F"));
    findAction -> setIcon(QIcon(":/images/find.png"));
    connect(findAction, SIGNAL(triggered()), collectW, SLOT(find()));

    findNextAction = new QAction(tr("Find &Next"), this);
    findNextAction -> setShortcut(tr("F3"));
    connect(findNextAction, SIGNAL(triggered()), collectW, SLOT(findNext()));

    findPreviousAction = new QAction(tr("Find &Previous"), this);
    findPreviousAction -> setShortcut(tr("Shift+F3"));
    connect(findPreviousAction, SIGNAL(triggered()), collectW, SLOT(findPrevious()));

    deleteContentsAction = new QAction(tr("&Delete"), this);
    deleteContentsAction -> setShortcut(tr("Del"));
    connect(deleteContentsAction, SIGNAL(triggered()), collectW, SLOT(deleteContents()));

    deleteEntireRowsAction = new QAction(tr("Delete Entire &Row(s)"), this);
    deleteEntireRowsAction -> setShortcut(tr("Ctrl+Del"));
    connect(deleteEntireRowsAction, SIGNAL(triggered()), collectW, SLOT(deleteEntireRows()));

    deleteEntireColumnsAction = new QAction(tr("Delete Entire C&olumn(s)"), this);
    deleteEntireColumnsAction -> setShortcut(tr("Ctrl+Shift+Del"));
    connect(deleteEntireColumnsAction, SIGNAL(triggered()), collectW, SLOT(deleteEntireColumns()));

    selectCurRowAction = new QAction(tr("Select Current Ro&w"), this);
    connect(selectCurRowAction, SIGNAL(triggered()), collectW, SLOT(selectCurrentRow()));

    selectCurColAction = new QAction(tr("Select Current Co&lumn"), this);
    connect(selectCurColAction, SIGNAL(triggered()), collectW, SLOT(selectCurrentColumn()));

    selectAllAction = new QAction(tr("Select &All"), this);
    selectAllAction -> setShortcut(tr("Ctrl+A"));
    connect(selectAllAction, SIGNAL(triggered()), collectW, SLOT(selectAll()));

    insertRowsAction = new QAction(tr("Insert &Rows"), this);
    insertRowsAction -> setShortcut(tr("Ctrl+Shift+R"));
    connect(insertRowsAction, SIGNAL(triggered()), collectW, SLOT(insertRows()));

    insertColumnsAction = new QAction(tr("Insert &Columns"), this);
    insertColumnsAction -> setShortcut(tr("Ctrl+Shift+C"));
    connect(insertColumnsAction, SIGNAL(triggered()), collectW, SLOT(insertColumns()));

    insertImageAction = new QAction(tr("Insert &Image..."), this);
    insertImageAction -> setShortcut(tr("Ctrl+I"));
    connect(insertImageAction, SIGNAL(triggered()), collectW, SLOT(insertImage()));

//    showGridAction = new QAction(tr("&Show Grid"), this);
//    showGridAction -> setShortcut(tr("Ctrl+G"));
//    showGridAction -> setCheckable(true);
//    showGridAction -> setChecked(collectW->showGrid());
//    connect(showGridAction, SIGNAL(toggled(bool)), collectW, SLOT(setShowGrid(bool)));
}

void ApplicationWindow::creatMenus()
{
    fileMenu = menuBar() -> addMenu(tr("&File"));
    fileMenu -> addAction(newCollectAction);
    fileMenu -> addAction(openCollectAction);
    fileMenu -> addAction(closeCollectAction);
    fileMenu -> addAction(addFeatureAction);
    fileMenu -> addAction(removeFeatureAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(saveAction);
    fileMenu -> addAction(saveAsAction);
    fileMenu -> addAction(saveFeatureAction);
//    fileMenu -> addAction(printAction);
    fileMenu -> addAction(printPDFAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(exitAction);

    editMenu = menuBar() -> addMenu(tr("&Edit"));
    editMenu -> addAction(undoAction);
    editMenu -> addAction(redoAction);
    editMenu -> addSeparator();
    editMenu -> addAction(copyAction);
    editMenu -> addAction(cutAction);
    editMenu -> addAction(pasteAction);
    editMenu -> addSeparator();
    editMenu -> addAction(findAction);
    editMenu -> addAction(findNextAction);
    editMenu -> addAction(findPreviousAction);
    editMenu -> addSeparator();
    editMenu -> addAction(deleteContentsAction);
    editMenu -> addAction(deleteEntireRowsAction);
    editMenu -> addAction(deleteEntireColumnsAction);
    editMenu -> addSeparator();
    editMenu -> addAction(selectAllAction);
    editMenu -> addAction(selectCurRowAction);
    editMenu -> addAction(selectCurColAction);
    editMenu -> addSeparator();
    editMenu -> addAction(insertRowsAction);
    editMenu -> addAction(insertColumnsAction);
//    editMenu -> addSeparator();
//    editMenu-> addAction(insertImageAction);

//    toolsMenu = menuBar() -> addMenu(tr("&Tools"));
//    toolsMenu -> addAction(sortAction);

//    optionsMenu = menuBar() -> addMenu(tr("&Options"));
//    optionsMenu -> addAction(showGridAction);
}

void ApplicationWindow::creatToolBars()
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

bool ApplicationWindow::saveFile(const QString &fileName)
{
    //   if(table->exportASCII(fileName))
       {
          setCurrentFile(fileName);
          setWindowModified(false);
          return true;
       }
    return false;
}

void ApplicationWindow::setCurrentFile(const QString &fullFileName)
{
    QString shownName = "Untitled";
    if (!fullFileName.isEmpty())
       shownName = QFileInfo(fullFileName).fileName();
    setWindowTitle(shownName + "[*] - The dataset of GPJson");
    currentFile = fullFileName;
}
