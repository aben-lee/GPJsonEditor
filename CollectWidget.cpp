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

#include "CollectWidget.h"
#include "FeatureWidget.h"
#include "gpjson.hpp"
#include "Commands.h"
#include "Dialogs/FindDialog.h"
#include "Dialogs/InsertRCDialog.h"
#include "Dialogs/NewFeatureDialog.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QFontDialog>
#include <QMenu>
#include <QUndoStack>
#include <QClipboard>

struct CollectWidgetData
{
    CollectWidgetData() : undoStack(0),findDialog(0)
    {}

    QString currentFile;
    GPJson::FeatureCollect feaCollect;
    QUndoStack *undoStack;
    FindDialog *findDialog;
};

CollectWidget::CollectWidget(QWidget *parent)
    : QTabWidget(parent), d(new CollectWidgetData)
{
    FeatureWidget *w = new FeatureWidget;
    w->setUpdatesEnabled(false);
    addTab(w,w->type());
    w->setUpdatesEnabled(true);
    d->feaCollect.list.append(w->feature());
    setWindowModified(true);

    setTabPosition(QTabWidget::South);
    setTabShape(QTabWidget::Triangular);
    setTabsClosable(true);
    connect(this,SIGNAL(tabCloseRequested(int)),this, SLOT(removeFeature(int)));

    d->undoStack = new QUndoStack(this);
}

CollectWidget::~CollectWidget()
{
    delete d->undoStack;
    delete d;
}

QUndoStack *CollectWidget::getUndoStack() const
{
   return d->undoStack;
}

bool CollectWidget::newFile()
{
    close();
    addFeature();

    return true;
}

bool CollectWidget::open(QString)
{
    //    setCurrentFile( table->importASCII());
    askToSaveChanges();

    d->currentFile = QFileDialog::getOpenFileName(this, tr("Import GPjson From File"),
                                     QString(), "GPJson files (*.gpjson *.GPJSON );;"
                                                 "Text files (*.TXT *.txt);;"
                                                  "Data files (*.DAT *.dat)");

    d->feaCollect.clear();
    for(int i = count(); i>=0 ; i--)
        delete widget(i);

    QFile file(d->currentFile);
    bool success = false;
    QFileInfo fileinfo = QFileInfo(d->currentFile);
    if (fileinfo.suffix().toLower() == "gpjson")
    {
        bool success = false;
        if (file.open(QIODevice::ReadOnly)) {
            success = loadGPjson(file.readAll());
            file.close();
        }
        else success = false;
    }
    else
    {
        // to do...
       // return _featureModel->exportASCII(fileName);
    }

    return success;
}

bool CollectWidget::close()
{
    askToSaveChanges();
//    save();
    setCurrentFile("");
    d->feaCollect.clear();
    for(int i = count(); i>=0 ; i--)
        delete widget(i);

    return true;
}

bool CollectWidget::addFeature()
{
    NewFeatureDialog dialog(this);
    if (dialog.exec())
    {
        int rowCount;
        int columnCount;
        GPJson::DatasetType type;
       if(dialog.radioButtonSheet->isChecked() || dialog.radioButtonMatrix->isChecked())
       {
           rowCount = dialog.rowsSpinBox -> value();
           columnCount = dialog.columnsSpinBox -> value();
           dialog.radioButtonSheet->isChecked()? type = GPJson::Sheet : GPJson::Matrix;
       }
       if(dialog.radioButtonBinary->isChecked() || dialog.radioButtonURI->isChecked())
       {
           rowCount = 1;
           columnCount = 1;
           dialog.radioButtonBinary->isChecked() ? type = GPJson::Binary : GPJson::URI;
       }

       FeatureWidget *w = new FeatureWidget;
       w->setUpdatesEnabled(false);
       addTab(w,w->type());
       w->setUpdatesEnabled(true);
       d->feaCollect.list.append(w->feature());
       setWindowModified(true);
       return true;
    }

    return false;
}

bool CollectWidget::removeFeature()
{
    if (!askToSaveChanges())
        return false;

    FeatureWidget *w = qobject_cast<FeatureWidget*>(currentWidget());
    if(w)
    {
        d->feaCollect.list.removeOne( w->feature() );
        delete w;
        return true;
    }

    return false;
}

bool CollectWidget::removeFeature(int index)
{
    setCurrentIndex(index);
    return removeFeature();
}

bool CollectWidget::save()
{
    if (d->currentFile.isEmpty())
       return saveAs();
    else
       return saveFile();
}

bool CollectWidget::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), ".",
                                                    "GPJson files (*.gpjson *.GPJSON );;"
                                                     "Text files (*.TXT *.txt);;"
                                                      "Data files (*.DAT *.dat)");

    if (!fileName.isEmpty())
    {
       setCurrentFile(fileName);
       return saveFile();
    }
    return false;
}

bool CollectWidget::saveFeature()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), ".",
                                                    "GPJson files (*.gpjson *.GPJSON );;"
/*                                                     "Text files (*.TXT *.txt);;"
                                                      "Data files (*.DAT *.dat)"*/);

    QFile file(fileName);
    bool success = false;
    QFileInfo fileinfo = QFileInfo(fileName);
    if (fileinfo.suffix().toLower() == "gpjson")
    {
        if (file.open(QIODevice::WriteOnly)) {
            FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
            if(!fw) return false;

            success = file.write(fw->feature()->toDocument().toJson());
            file.close();
        }
        else success = false;
    }
    else
    {
        // to do... 存文本格式
//        _featureModel->exportASCII(fileName);
    }

    return success;
}

void CollectWidget::print()
{
    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
    if(!fw) return;

    fw->print();
}

void CollectWidget::printPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), ".",
                                                    "GPJson files (*.pdf *.PDF )");

    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
    if(fw) {
        fw->exportPDF(fileName);
    }
}

void CollectWidget::cut()
{
    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
    if(!fw) return;

    getUndoStack() -> push(new CutCommand(fw));
}

void CollectWidget::copy()
{
    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
    if(!fw) return;

    QApplication::clipboard()->setText(QString(fw -> getItemsInByteArray(fw->getSelected())),
                                       QClipboard::Selection);

    QMimeData *mimeData = new QMimeData;
    fw -> copy(*mimeData);
    QApplication::clipboard()->setMimeData(mimeData);
}

void CollectWidget::paste()
{
   FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
   if(!fw) return;

   getUndoStack() -> push(new PasteCommand(fw));
}

void CollectWidget::find()
{
    if (!d->findDialog)
    {
       d->findDialog = new FindDialog(this);
       connect(d->findDialog, SIGNAL(findNextSignal()), this, SLOT(findNext()));
       connect(d->findDialog, SIGNAL(findPreviousSignal()), this, SLOT(findPrevious()));
    }
    d->findDialog -> exec();
}

void CollectWidget::findNext()
{
    if (!d->findDialog)
       find();
    else
    {
       FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
       if(fw) {
           QString text = d->findDialog->text();
           fw -> findNext(text, d->findDialog->caseSensitivity());
       }
    }
}

void CollectWidget::findPrevious()
{
    if (!d->findDialog)
       find();
    else
    {
       FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
        if(fw) {
           QString text = d->findDialog->text();
           fw -> findPrevious(text, d->findDialog->caseSensitivity());
        }
    }
}

void CollectWidget::deleteContents()
{
   FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
   if(!fw) return;
   d->undoStack->push(new DeleteContents(fw));
}

void CollectWidget::deleteEntireRows()
{
    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
        if(!fw) return;

    QItemSelection select = fw->getSelected();
    int position = select.indexes().first().row();
    int amount = select.indexes().last().row()-position;
    d->undoStack->push(new DeleteEntireRows(fw, position, amount));
}

void CollectWidget::deleteEntireColumns()
{
    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
        if(!fw) return;
    QItemSelection select = fw->getSelected();
    int position = select.indexes().first().row();
    int amount = select.indexes().last().row()-position;
    d->undoStack->push(new DeleteEntireColumns(fw, position, amount));
}

void CollectWidget::selectAll()
{
    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
    if(!fw) return;

    fw->selectAll();
}

void CollectWidget::selectCurrentRow()
{
    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
    if(!fw) return;

    fw->selectCurrentRow();
}

void CollectWidget::selectCurrentColumn()
{
    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
    if(!fw) return;

    fw->selectCurrentColumn();
}

void CollectWidget::insertRows()
{
    InsertRCDialog dialog(InsertRCDialog::InsertRows, this);
    if (dialog.exec())
    {
        FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
        if(fw) {
           int amount = dialog.amountSpinBox -> value();
           int position = dialog.afterRadioButton ->
                          isChecked() ? FeatureWidget::InsertAfter : FeatureWidget::InsertBefore;
           fw -> getUndoStack() -> push(new InsertRowsCommand(fw, position, amount));
        }
    }
}

void CollectWidget::insertColumns()
{
    InsertRCDialog dialog(InsertRCDialog::InsertColumns, this);
    if (dialog.exec())
    {
        FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
        if(fw) {
           int amount = dialog.amountSpinBox -> value();
           int position = dialog.afterRadioButton ->
                          isChecked() ? FeatureWidget::InsertAfter : FeatureWidget::InsertBefore;
           fw -> getUndoStack() -> push(new InsertColumnsCommand(fw, position, amount));
        }
    }

}

void CollectWidget::insertImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), ".",
                                                    tr("All Files (*.*)"));
    if (fileName.isEmpty())
       return;

    QImage image;
    if (!image.load(fileName))
    {
       QMessageBox::warning(this, tr("Opening failed"), tr("Unknown file format!"));
       return;
    }
    FeatureWidget *fw = qobject_cast<FeatureWidget*>(currentWidget());
    if(fw) {
//    int xCoordinate = fw -> columnViewportPosition(fw->currentColumn());
//    int yCoordinate = fw -> rowViewportPosition(fw->currentRow());
//    fw -> getUndoStack() -> push(new InsertImageCommand(fw, image, xCoordinate, yCoordinate));
    }
}

void CollectWidget::sort()
{
    // to do..
}

void CollectWidget::closeEvent(QCloseEvent *event)
{
    if (askToSaveChanges())
       event -> accept();
    else
        event -> ignore();
}

bool CollectWidget::askToSaveChanges()
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

bool CollectWidget::saveFile()
{
    if(d->currentFile.isEmpty())
    {
        d->currentFile = QFileDialog::getSaveFileName(this, tr("Save As"), ".",
                                                    "GPJson files (*.gpjson *.GPJSON );;"
                                                     "Text files (*.TXT *.txt);;"
                                                      "Data files (*.DAT *.dat)");
    }

    QFile file(d->currentFile);
    bool success = false;
    QFileInfo fileinfo = QFileInfo(d->currentFile);
    if (fileinfo.suffix().toLower() == "gpjson")
    {
        if (file.open(QIODevice::WriteOnly)) {
            success = file.write(d->feaCollect.toDocument().toJson());
            file.close();
        }
        else success = false;

    }
    else
    {
        // to do... 存文本格式
//        _featureModel->exportASCII(fileName);
    }

    return success;
}

void CollectWidget::setCurrentFile(const QString &fullFileName)
{
    QString shownName = "Untitled";
    if (!fullFileName.isEmpty())
       shownName = QFileInfo(fullFileName).fileName();
    setWindowTitle(shownName + "[*] - The dataset of GPJson");
    d->currentFile = fullFileName;
}

bool CollectWidget::loadGPjson(const QByteArray &json)
{
    QString errorInfo;
    d->feaCollect.clear();
    bool success = GPJson::parse(QJsonDocument::fromJson(json),d->feaCollect, errorInfo );

    if(success)
    {
        for (auto f : d->feaCollect.list)
        {
            FeatureWidget *w = new FeatureWidget(f,this);
            if(w) {
                w->setUpdatesEnabled(false);
                addTab(w,w->type());
                w->setUpdatesEnabled(true);
            }
        }
    }
    else
    {
        qDebug() << "loading the gpjson error :" << errorInfo;
    }

    return success;
}

void CollectWidget::propEditPressed()
{

    QDialog infoDialog(this);

    ui.setupUi(&infoDialog);
    Qt::WindowFlags flags = Qt::Tool;
    infoDialog.setWindowFlags(flags);
//    ui.idEdit->setText(d->feaModel->id());
//    ui.typeEdit->setText(d->feaModel->type());
//    ui.treeView->loadJson(QJsonDocument(d->feaModel->properties()).toJson());
//    ui.treeView->setVisible(false);

//    connect(ui.applyBtn,SIGNAL(clicked()),this,SLOT(updateProp()));

    infoDialog.exec();
}
