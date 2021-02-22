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

#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QMainWindow>
#include <QAction>
#include "CollectWidget.h"

class ApplicationWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ApplicationWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
        ~ApplicationWindow();

private slots:
   void documentModified();
   void updateStatusBar();
   void updateActions();

signals:

private:
   void creatActions();
   void creatMenus();
   void creatToolBars();
//   void creatStatusBar();
//   void creatContextMenu();
   bool saveFile(const QString &fileName);
   void setCurrentFile(const QString &);

   QString currentFile;
   CollectWidget *collectW;

   //File Menu Actions
   QAction *newCollectAction;
   QAction *openCollectAction;
   QAction *closeCollectAction;
   QAction *addFeatureAction;
   QAction *removeFeatureAction;
   QAction *saveAction;        // 保存FeatureCollect
   QAction *saveAsAction;      // 另存FeatureCollect
   QAction *saveFeatureAction;    // 保存当前的Feature 到独立文件
   QAction *printAction;
   QAction *printPDFAction;
   QAction *exitAction;

   QAction *undoAction;
   QAction *redoAction;
   QAction *copyAction;
   QAction *cutAction;
   QAction *pasteAction;
   QAction *findAction;
   QAction *findNextAction;
   QAction *findPreviousAction;
   QAction *deleteContentsAction;
   QAction *deleteEntireRowsAction;
   QAction *deleteEntireColumnsAction;
   QAction *selectCurRowAction;
   QAction *selectCurColAction;
   QAction *selectAllAction;

   QAction *insertRowsAction;
   QAction *insertColumnsAction;
   QAction *insertImageAction;

//   QAction *textLineFormatAction;
//   QAction *dateFormatAction;
//   QAction *timeFormatAction;
//   QAction *integerFormatAction;
//   QAction *decimalFormatAction;
//   QAction *fontAction;
//   QAction *alignLeftAction;
//   QAction *alignCenterAction;
//   QAction *alignRightAction;
//   QAction *backgroundAction;
//   QAction *foregroundAction;

//   QAction *sortAction;

//   QAction *showGridAction;

//   QAction *functionListAction;


   ///Menus
   QMenu *fileMenu;
   QMenu *editMenu;
//   QMenu *insertMenu;
//   QMenu *toolsMenu;
   QMenu *optionsMenu;
//   QMenu *helpMenu;

   ///Toolbars
   QToolBar *mainToolBar;
//   QToolBar *featuresToolBar;
};

#endif // APPLICATIONWINDOW_H
