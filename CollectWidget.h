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

#ifndef COLLECTWIDGET_H
#define COLLECTWIDGET_H

#include <QTabWidget>
#include <QSharedDataPointer>
#include <QWidget>
#include "ui_FeatureInfo.h"

class QUndoStack;
class QMimeData;

struct CollectWidgetData;
class CollectWidget : public QTabWidget
{
    Q_OBJECT
public:
    CollectWidget(QWidget *parent = nullptr);
        ~CollectWidget();

    QUndoStack *getUndoStack() const;

public slots:
   bool newFile();
   bool open(QString = QString());
   bool close();
   bool addFeature();
   bool removeFeature();
   bool removeFeature(int index);
   bool save();
   bool saveAs();
   bool saveFeature();
   void print();
   void printPDF();

   void cut();
   void copy();
   void paste();
   void find();
   void findNext();
   void findPrevious();
   void deleteContents();
   void deleteEntireRows();
   void deleteEntireColumns();

   void selectAll();
   void selectCurrentRow();
   void selectCurrentColumn();
   void insertRows();
   void insertColumns();
   void insertImage();

   void sort();

protected:
   void closeEvent(QCloseEvent*) override;
    
private:
   bool askToSaveChanges();
   bool saveFile();
   void setCurrentFile(const QString&);
   bool loadGPjson(const QByteArray &json);
   void propEditPressed();

private:
    CollectWidgetData *d;
    Ui::FeatureInfo ui;
};

#endif // COLLECTWIDGET_H
