

#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QMainWindow>
#include "gpjson.hpp"

class QLabel;
class QAction;
class FindDialog;
class QLineEdit;
class QMessageBox;
class FunctionListDialog;
class Graphic;
class SortDialog;
class FeatureWidget;

class CollectWindow : public QMainWindow
{
   Q_OBJECT
   
   public:
      CollectWindow(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
        ~CollectWindow();

   protected:
      void closeEvent(QCloseEvent*);
      
   public slots:
      bool newFile();
      bool open(QString = QString());
      bool addFeature();
      bool save();
      bool saveAs();
      
   private slots:
      void copy();
      void cut();
      void paste();
      void find();
      void findNext();
      void findPrevious();
      void deleteContents();
      void deleteEntireRows();
      void deleteEntireColumns();
      
      void insertRows();
      void insertColumns();
            
      void textLineFormat();
      void dateFormat();
      void timeFormat();
      void integerFormat();
      void decimalFormat();
      void font();
      void align(bool);
      
      void sort();
      
      void userChangedItemDataSlot(int, int, QVariant &, QVariant &);
      void documentModified();
      void updateStatusBar();
      void updateActions();
      
   private:
      QString currentFile;
      GPJson::FeatureCollect feaCollect;
      QTabWidget *feaWidgets;    // for FeatureCollect;

      FindDialog *findDialog;
      QAction *tableInfoEdit;
      QLabel *currentCellDataLabel;
      FunctionListDialog *functionListDialog;
      
      //File Menu Actions
      QAction *newCollectAction;
      QAction *openCollectAction;
      QAction *addFeatureAction;
      QAction *exitAction;


//      QAction *removeFeatureAction;
//      QAction *closeCollectAction;
//      QAction *saveAction;        // 保存FeatureCollect
//      QAction *saveAsAction;      // 另存FeatureCollect
//      QAction *saveFeatureToFileAction;    // 保存当前的Feature 到独立文件
      
//      QAction *undoAction;
//      QAction *redoAction;
//      QAction *copyAction;
//      QAction *cutAction;
//      QAction *pasteAction;
//      QAction *findAction;
//      QAction *findNextAction;
//      QAction *findPreviousAction;
//      QAction *deleteContentsAction;
//      QAction *deleteEntireRowsAction;
//      QAction *deleteEntireColumnsAction;
//      QAction *selectCurRowAction;
//      QAction *selectCurColAction;
//      QAction *selectAllAction;      
//      QAction *insertRowsAction;
//      QAction *insertColumnsAction;
      
//      QAction *textLineFormatAction;
//      QAction *dateFormatAction;
//      QAction *timeFormatAction;
//      QAction *integerFormatAction;
//      QAction *decimalFormatAction;
//      QAction *fontAction;
//      QAction *alignLeftAction;
//      QAction *alignCenterAction;
//      QAction *alignRightAction;
      
      QAction *sortAction;
      
//      QAction *showGridAction;
      
      
      ///Menus
      QMenu *fileMenu;
      QMenu *editMenu;
      QMenu *insertMenu;
//      QMenu *formatMenu;
      QMenu *toolsMenu;
      QMenu *optionsMenu;
      QMenu *helpMenu;
      
      ///Toolbars
      QToolBar *mainToolBar;
      QToolBar *featuresToolBar;
      
      ///Private Functions
      void creatActions();
      void creatMenus();
      void creatToolBars();
      void creatStatusBar();
      void creatContextMenu();
      bool askToSaveChanges();
      bool saveFile(const QString&);
      void setCurrentFile(const QString&);
};

#endif



