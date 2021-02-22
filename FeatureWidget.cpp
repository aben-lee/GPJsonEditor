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
#include "FeatureWidget.h"
#include "FeatureModel.h"
#include "Delegate.h"
#include <QVariant>
#include <QTreeView>
#include <QCursor>
#include <QHoverEvent>
#include <QMenu>
#include <QDialog>
#include <QFileDialog>
#include <QUndoStack>
#include <QComboBox>
#include <QJsonDocument>
#include <QKeyEvent>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QModelIndex>
#include <QMessageBox>
#include <QTextEdit>
#include "Dialogs/NewFeatureDialog.h"
#include "Graphic.h"
#include "PictureGraphic.h"

//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
**TableHeaderView added the GPJson properties menu
** **************************************************************************/

TableHeaderView::TableHeaderView(Qt::Orientation orientation, QWidget * parent)
    : QHeaderView(orientation, parent)
{
    // Required to refresh button menu when the mouse leave the header.
    setAttribute(Qt::WA_Hover, true);
}

TableHeaderView::~TableHeaderView()
{
}

void TableHeaderView::mousePressEvent ( QMouseEvent * event )
{
    QHeaderView::mousePressEvent(event);

    int logicalIndex = logicalIndexAt(event->pos());

    if (buttonMenuRect(logicalIndex).contains(event->pos())) {
        QMenu menu(this);
        QAction *hideAction = menu.addAction(tr("Hide Column"));
        QAction *readOnlyAction = menu.addAction(tr("Read Only"));
//        QAction *textAction = menu.addAction(tr("Text Type"));
        QAction *integerAction = menu.addAction(tr("Integer Type"));
        QAction *decimalAction = menu.addAction(tr("Decimal Type"));
        QAction *dateTimeAction = menu.addAction(tr("DateTime Type"));
//        QAction *dateAction = menu.addAction(tr("Date Type"));
        QAction *timeAction = menu.addAction(tr("Time Type"));
//        QAction *boolAction = menu.addAction(tr("Bool Type"));
//        QAction *sortAZ = menu.addAction("Sort sheet A->Z");
//        QAction *sortZA = menu.addAction("Sort sheet Z->A");

        // Disable hide column if only one column remains. Otherwise
        // the gui is no more available to show them back.
        hideAction->setEnabled(hiddenSectionCount() < count() - 1);

        QAction *res = menu.exec(mapToGlobal(event->pos()));

        if (res == hideAction) {
            hideSection(logicalIndex);
            updateSection(logicalIndex-1);
        }
        if(res == readOnlyAction){
            emit changeColumnEditor(logicalIndex, FeatureWidget::ReadOnlyType);
        }
//        if(res == textAction){
//            emit changeColumnEditor(logicalIndex, TableView::TextLineType);
//        }
        if(res == integerAction){
            emit changeColumnEditor(logicalIndex, FeatureWidget::IntegerType);
        }
        if(res == decimalAction){
            emit changeColumnEditor(logicalIndex, FeatureWidget::DecimalType);
        }
        if(res == dateTimeAction){
            emit changeColumnEditor(logicalIndex, FeatureWidget::DateTimeType);
        }
//        if(res == dateAction){
//            emit changeColumnEditor(logicalIndex, TableView::DateType);
//        }
        if(res == timeAction){
            emit changeColumnEditor(logicalIndex, FeatureWidget::TimeType);
        }
//        if(res == boolAction){
//            emit changeColumnEditor(logicalIndex, TableView::BoolType);
//        }
//        if (res == sortAZ)
//            model()->sort(logicalIndex, Qt::AscendingOrder);
//        if (res == sortZA)
//            model()->sort(logicalIndex, Qt::DescendingOrder);
    }

    // Catch previous arrow mouse click.
    if (prevRect(logicalIndex).contains(event->pos())) {
        showSection(logicalIndex - 1);
        updateSection(logicalIndex - 2);
    }

    // Catch next arrow mouse click.
    if (nextRect(logicalIndex).contains(event->pos())) {
        showSection(logicalIndex + 1);
        updateSection(logicalIndex + 2);
    }
}

void TableHeaderView::mouseMoveEvent(QMouseEvent * event)
{
    QHeaderView::mouseMoveEvent(event);

    // Required to refresh the button menu enable/disable state.
    int logicalIndex = logicalIndexAt(event->pos());
    updateSection(logicalIndex);
}

void TableHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();

    QHeaderView::paintSection(painter, rect, logicalIndex);

    painter->restore();

    if (!rect.isValid() || isSortIndicatorShown())
        return;

    if (isSectionHidden(logicalIndex - 1)) {
        drawPrevButton(painter, logicalIndex);
    }

    if (isSectionHidden(logicalIndex + 1)) {
        drawNextButton(painter, logicalIndex);
    }

    QPoint pos = mapFromGlobal(QCursor::pos());
    if (rect.contains(pos)) {
        drawMenuButton(painter, logicalIndex, buttonMenuRect(logicalIndex).contains(pos));
    }
}

QRect TableHeaderView::sectionRect(int logicalIndex) const
{
    return QRect(sectionViewportPosition(logicalIndex), 0, sectionSize(logicalIndex), height());
}

QRect TableHeaderView::buttonMenuRect(int logicalIndex) const
{
    QRect sr = sectionRect(logicalIndex);

    return QRect(sr.right() - 5 - 13, sr.center().y() - 6, 13, 13);
}

QRect TableHeaderView::prevRect(int logicalIndex) const
{
    if (isSectionHidden(logicalIndex))
        return QRect();

    QRect sr = sectionRect(logicalIndex);

    return QRect(sr.left() + 1, sr.center().y() - 6, 13, 13);
}

QRect TableHeaderView::nextRect(int logicalIndex) const
{
    if (isSectionHidden(logicalIndex))
        return QRect();

    QRect sr = sectionRect(logicalIndex);

    return QRect(sr.right() - 13, sr.center().y() - 6, 13, 13);
}

void TableHeaderView::drawMenuButton(QPainter *painter, int logicalIndex, bool enabled) const
{
    QRect brect = buttonMenuRect(logicalIndex);

    painter->setPen(enabled ? QColor(186,186,186) : QColor(223, 223, 223));
    painter->setBrush(QColor(246,246,246));
    painter->drawRect(brect.adjusted(0,0,-1,-1));

    painter->setPen(enabled ? QColor(71,71,71) : QColor(193, 193, 193));
    painter->drawLine(brect.left()+3, brect.top()+5, brect.right()-3, brect.top()+5);
    painter->drawLine(brect.left()+4, brect.top()+6, brect.right()-4, brect.top()+6);
    painter->drawLine(brect.left()+5, brect.top()+7, brect.right()-5, brect.top()+7);
    painter->drawPoint(brect.left()+6, brect.top()+8);
}

void TableHeaderView::drawPrevButton(QPainter *painter, int logicalIndex) const
{
    QRect rect = prevRect(logicalIndex);

    painter->setPen(QColor(71,71,71));
    painter->drawLine(rect.left()+1, rect.center().y() - 3, rect.left()+1, rect.center().y() + 3);
    painter->drawLine(rect.left()+2, rect.center().y() - 2, rect.left()+2, rect.center().y() + 2);
    painter->drawLine(rect.left()+3, rect.center().y() - 1, rect.left()+3, rect.center().y() + 1);
    painter->drawPoint(rect.left()+4, rect.center().y());
}

void TableHeaderView::drawNextButton(QPainter *painter, int logicalIndex) const
{
    QRect rect = nextRect(logicalIndex);

    painter->setPen(QColor(71,71,71));
    painter->drawLine(rect.right()-2, rect.center().y() - 3, rect.right()-2, rect.center().y() + 3);
    painter->drawLine(rect.right()-3, rect.center().y() - 2, rect.right()-3, rect.center().y() + 2);
    painter->drawLine(rect.right()-4, rect.center().y() - 1, rect.right()-4, rect.center().y() + 1);
    painter->drawPoint(rect.right()-5, rect.center().y());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

struct FeatureWidgetData
{
    FeatureWidgetData() : feaModel(0)/*,textEidit(0)*/,undoStack(0),selectedGraphic(0)
    {}

    FeatureModel *feaModel;

    QUndoStack *undoStack;
    int indexF;
    QList<Graphic*> graphics;
    Graphic* selectedGraphic;
//    QAction *_infoEditAction,*_openGPJsonAction,*_saveGPJsonAction;
//    QAction *_printerAction;
//    Ui::FeatureInfo ui;
};

FeatureWidget::FeatureWidget(QWidget *parent)
    : QTableView(parent)
{
    // 初始化
    d = new FeatureWidgetData;
    d->feaModel = new FeatureModel(this);
    setModel(d->feaModel);
    d->feaModel->setReadOnly(false);
    connect(d->feaModel,SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
                this, SLOT(updateDelegate(Qt::Orientation, int, int)));

    d->undoStack = new QUndoStack(this);
    init();
    //    newSheet();
}

FeatureWidget::FeatureWidget(QSharedPointer<GPJson::Feature> feature, QWidget *parent)
    : QTableView(parent)
{
    d = new FeatureWidgetData;
    d->feaModel = new FeatureModel(feature,this);
    setModel(d->feaModel);
    d->feaModel->setReadOnly(false);
    connect(d->feaModel,SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
                this, SLOT(updateDelegate(Qt::Orientation, int, int)));

    d->undoStack = new QUndoStack(this);
    init();
}

FeatureWidget::~FeatureWidget()
{
    delete d->feaModel;
    delete d->undoStack;
    delete d;
}

bool FeatureWidget::readOnly() const
{
    return d->feaModel->readOnly();
}

void FeatureWidget::setReadOnly(const bool state)
{
    return d->feaModel->setReadOnly(state);
}

QString FeatureWidget::type() const
{
    return d->feaModel->type();
}

void FeatureWidget::setType(const QString &t)
{
    return d->feaModel->setType(t);
}

QString FeatureWidget::id() const
{
    return d->feaModel->id();
}

void FeatureWidget::setID(const QString &bytes)
{
    d->feaModel->setID(bytes);
}

QJsonObject FeatureWidget::properties() const
{
    return d->feaModel->properties();
}

void FeatureWidget::setProperties(const QJsonObject prop)
{
    d->feaModel->setProperties(prop);
}

QSharedPointer<GPJson::Feature> FeatureWidget::feature()
{
    return d->feaModel->feature();
}

void FeatureWidget::setFeature(GPJson::Feature *fe)
{
    return d->feaModel->setFeature(fe);
}

bool FeatureWidget::setFeature(const QByteArray &json, QString &errorInfo)
{
    return d->feaModel->setFeature(json,errorInfo);
}

////bool FeatureWidget::setFeature(GPJson::Feature *fe)
////{
////    if(!fe)
////    {
////        QMessageBox::information(this, tr(""),
////                                       tr("The new GPJSON Feature is Null."),
////                                       QMessageBox::Ok);
////        return false;
////    }

////    d->feaModel->setFeature(fe);
////    return true;
////}

////void FeatureWidget::resetFeatureCollect()
////{
////    if (!askToSaveChanges())
////        return;

////    d->featureCollect.clear();
////    d->feaModel.reset();
////}

//void FeatureWidget::newSheet(const int rowCount, const int columnCount,const GPJson::DatasetType &type)
//{
////    disconnect(_featuresComboBox, SIGNAL(currentIndexChanged(int)),
////             this, SLOT(currentFeatureSelect(int)));
//////    _featuresComboBox->clear();
////    connect(_featuresComboBox, SIGNAL(currentIndexChanged(int)),
////             this, SLOT(currentFeatureSelect(int)));


////    d->feaModel->reset();
//    d->feaModel->insertRows(0,rowCount-1);
//    d->feaModel->insertColumns(0,columnCount-1);

////    d->feaModel->insertColumns(0,2);
////    _features.list.append(d->feaModel->feature());

////    adjustHorizontalHeader();
//    //    undoStack -> clear();
//}

//bool FeatureWidget::writeFeature(const QString & fileName)
//{
//    QFile file(fileName);
//    if (!file.open(QIODevice::WriteOnly))
//    {
//       QMessageBox::warning(this, tr("Saving Failed"),
//                            tr("Cannot write to file %1:\n%2")
//                            .arg(fileName).arg(file.errorString()));
//       return false;
//    }

//    setCursor(Qt::WaitCursor);
////    file.write(d->feaModel->feature()->toDocument().toJson());
//    file.close();
//    unsetCursor();
//    return true;
//}

//bool FeatureWidget::readFeature(const QString &fileName)
//{
//    QFile file(fileName);
//    if(!file.open(QIODevice::ReadOnly))
//    {
//       QMessageBox::warning(this, tr("Reading Failed"),tr("Cannot read the file %1 :\n%2")
//                            .arg(fileName).arg(file.errorString()));
//       return false;
//    }

//   setCursor(Qt::WaitCursor);
//   QString errorInfo = "";
////   bool sucess = d->feaModel->setFeature(file.readAll(),errorInfo);
////   file.close();
////   unsetCursor();
////   return sucess;
//}

void FeatureWidget::changeCellEditor(int column, FeatureWidget::ColumnType type)
{
    if(type == ReadOnlyType)
        setItemDelegateForColumn(column, new ReadOnlyDelegate(this));
    if(type == TextLineType)
        setItemDelegateForColumn(column, new TextItemDelegate(this));
    if(type == IntegerType)
        setItemDelegateForColumn(column, new SpinBoxDelegate(this));
    if(type == DecimalType)
        setItemDelegateForColumn(column, new DoubleSpinBoxDelegate(this));
    if(type == DateTimeType)
        setItemDelegateForColumn(column, new DateTimeDelegate(this));
    if(type == TimeType)
        setItemDelegateForColumn(column, new TimeDelegate(this));
}

////void FeatureWidget::setFeatureModel(FeatureModel *model)
////{
////    _tableView->setModel(model);
////    emit modelModified();
////}

////bool FeatureWidget::loadGPjsonFile(const QString &fn)
////{
////    QString fileName;
////    if (fn.isEmpty())
////        fileName = QFileDialog::getOpenFileName(this, tr("Import GPjson From File"),
////                                         QString(), tr("GPJson (*.gpjson *.GPJSON )"));
////    else
////        fileName = fn;

////    if (fileName.isEmpty()) return false;
////    QFile file(fileName);
////    bool success = false;
////    if (file.open(QIODevice::ReadOnly)) {
////        success = loadGPjson(&file);
////        file.close();
////    }
////    else success = false;
////    return  success;
////}

////bool FeatureWidget::loadGPjson(QIODevice *device)
////{
////    return loadGPjson(device->readAll());
////}

////bool FeatureWidget::loadGPjson(const QByteArray &json)
////{
////     d->feaModel->reset();
////     _features.clear();

////     QString errorInfo;
////    if(_features.fromDocument(QJsonDocument::fromJson(json), errorInfo ))
////    {

////        disconnect(_featuresComboBox, SIGNAL(currentIndexChanged(int)),
////                 this, SLOT(currentFeatureSelect(int)));
////        _featuresComboBox->clear();
////        for (int i = 0 ; i < _features.list.count(); ++i) {
////            _featuresComboBox->addItem( QString("GPJson %1 of %2").arg(i+1).arg(_features.list.count()));
////        }
////        connect(_featuresComboBox, SIGNAL(currentIndexChanged(int)),
////                 this, SLOT(currentFeatureSelect(int)));
////        _featuresComboBox->activated(_featuresComboBox->currentIndex());
//////    d->feaModel->setFeature(_features.list.at(_featuresComboBox->currentIndex()));   // have been connect the _featuresComboBox->activated

////        return true;
////    }
////    return false;
////}

//bool FeatureWidget::saveGPjsonFile(const QString &fn)
//{
//    QString fileName;
//    if (fn.isEmpty())
//        fileName = QFileDialog::getSaveFileName(this, tr("Export GPJson To File"),
//                                         QString(), tr("Json (*.gpjson *.GPJSON );;"));
//    else
//        fileName = fn;

//    if (fileName.isEmpty()) return false;
//    QFile file(fileName);
//    bool success = false;
//    if (file.open(QIODevice::WriteOnly)) {
//        success = saveGPjson(&file);
//        file.close();
//    }
//    else success = false;
//    return  success;
//}

bool FeatureWidget::saveGPjson(QIODevice *device)
{
    return device->write(feature()->toDocument().toJson());
}

void FeatureWidget::printReview()
{
    QPrinter printer(QPrinter::HighResolution);
    // 创建打印预览对话框
    QPrintPreviewDialog preview(&printer, this,
                                windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    // 当要生成预览页面时，发射paintRequested()信号
    connect(&preview, SIGNAL(paintRequested(QPrinter *)),
            this, SLOT(printPreview(QPrinter *)));
    preview.exec();
}

void FeatureWidget::print()
{
    print(QString());
}

void FeatureWidget::print(const QString &fileName)
{
    QPrinter printer;
    printer.setColorMode (QPrinter::GrayScale);

    if (!fileName.isEmpty())
    {
        printer.setCreator("GeoArch");
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
    }
    else
    {
        QPrintDialog printDialog(&printer);
        if (printDialog.exec() != QDialog::Accepted)
            return;
    }

    printer.setFullPage( true );
    QPainter p;
    if ( !p.begin(&printer ) )
        return; // paint on printer
    int dpiy = printer.logicalDpiY();
    const int margin = (int) ( (1/2.54)*dpiy ); // 1 cm margins

    QHeaderView *hHeader = horizontalHeader();
    QHeaderView *vHeader = verticalHeader();

    int rows = rowCount();
    int cols = columnCount();
    int height = margin;
    int i, vertHeaderWidth = vHeader->width();
    int right = margin + vertHeaderWidth;

    // print header
    p.setFont(hHeader->font());
    QString header_label = model()->headerData(0, Qt::Horizontal).toString();
    QRect br = p.boundingRect(br, Qt::AlignCenter, header_label);
    p.drawLine(right, height, right, height+br.height());
    QRect tr(br);

    for (i=0;i<cols;i++)
    {
        int w = columnWidth(i);
        tr.setTopLeft(QPoint(right,height));
        tr.setWidth(w);
        tr.setHeight(br.height());
        header_label = model()->headerData(i, Qt::Horizontal).toString();
        p.drawText(tr, Qt::AlignCenter, header_label);
        right += w;
        p.drawLine(right, height, right, height+tr.height());

        if (right >= printer.width()-2*margin )
            break;
    }

    p.drawLine(margin + vertHeaderWidth, height, right-1, height);//first horizontal line
    height += tr.height();
    p.drawLine(margin, height, right-1, height);

    // print table values
    for (i=0;i<rows;i++)
    {
        right = margin;
        QString cell_text = model()->headerData(i, Qt::Vertical).toString()+"\t";
        tr = p.boundingRect(tr, Qt::AlignCenter, cell_text);
        p.drawLine(right, height, right, height+tr.height());

        br.setTopLeft(QPoint(right,height));
        br.setWidth(vertHeaderWidth);
        br.setHeight(tr.height());
        p.drawText(br, Qt::AlignCenter, cell_text);
        right += vertHeaderWidth;
        p.drawLine(right, height, right, height+tr.height());

        for(int j=0;j<cols;j++)
        {
            int w = columnWidth (j);
            cell_text = at(i,j).toString()+"\t";
            tr = p.boundingRect(tr,Qt::AlignCenter,cell_text);
            br.setTopLeft(QPoint(right,height));
            br.setWidth(w);
            br.setHeight(tr.height());
            p.drawText(br, Qt::AlignCenter, cell_text);
            right += w;
            p.drawLine(right, height, right, height+tr.height());

            if (right >= printer.width()-2*margin )
                break;
        }
        height += br.height();
        p.drawLine(margin, height, right-1, height);

        if (height >= printer.height()-margin )
        {
            printer.newPage();
            height = margin;
            p.drawLine(margin, height, right, height);
        }
    }
}

void FeatureWidget::exportPDF(const QString &fileName)
{
    print(fileName);
}

void FeatureWidget::setCurrentCell(const int row, const int column)
{
    selectRow(row);
    selectColumn(column);
}

void FeatureWidget::selectCurrentRow()
{
    selectRow(currentRow());
}

void FeatureWidget::selectCurrentColumn()
{
    selectColumn(currentColumn());
}

void FeatureWidget::findNext(QString &text, Qt::CaseSensitivity cs)
{
    if (text.isEmpty())
       return;

    int initialRow = currentRow();
    int initialColumn = currentColumn();
    int i=initialRow;
    int j=initialColumn;
    bool found = false;

    do
    {
       j++;
       if (j >= columnCount())
       {
          j=0;
          i++;
       }
       if (i >= rowCount())
          i=0;

       QString itemText = at(i, j).toString() ;
       if (itemText.contains(text, cs))
       {
          found = true;
          break;
       }
    } while(i!=initialRow || j!=initialColumn);

    if (found)
        setCurrentCell(i,j);
}

void FeatureWidget::findPrevious(QString &text, Qt::CaseSensitivity cs)
{
    if (text.isEmpty())
       return;

    int initialRow = currentRow();
    int initialColumn = currentColumn();
    int i=initialRow;
    int j=initialColumn;
    bool found = false;

    do
    {
       j--;
       if (j < 0)
       {
          j=columnCount()-1;
          i--;
       }
       if (i < 0)
          i=rowCount()-1;

       QString itemText = at(i, j).toString() ;
       if (itemText.contains(text, cs))
       {
          found = true;
          break;
       }
    } while(i!=initialRow || j!=initialColumn);

    if (found)
       setCurrentCell(i,j);
}

QUndoStack *FeatureWidget::getUndoStack() const
{
    return d->undoStack;
}

QItemSelection FeatureWidget::getSelected()
{
     return selectionModel()->selection();
}

void FeatureWidget::cut(QMimeData &mimeData)
{
    copy(mimeData);
    if (d->selectedGraphic == 0)
       deleteContents(getSelected());
    else
       deleteGraphic(d->selectedGraphic);
}

void FeatureWidget::copy(QMimeData &mimeData)
{
    if (d->selectedGraphic == 0)
    {
       QItemSelection range = getSelected();
       mimeData.setData(CELL_TYPE, getItemsInByteArray(range));
    }
    else
       mimeData.setData(GRAPHIC_TYPE, getGraphicInByteArray(getSelectedGraphic()));
}

bool FeatureWidget::paste(const QMimeData &mimeData, int row, int column)
{
    if (mimeData.hasFormat(CELL_TYPE))
    {
       QByteArray byteArray = mimeData.data(CELL_TYPE);
       QDataStream inStream(&byteArray, QIODevice::ReadOnly);
       quint32 magic;
       inStream >> magic;
       if (magic != 0xA0B0C0D0)   // Read and check the header
           return false;   //XXX_BAD_FILE_FORMAT
       inStream.setVersion(QDataStream::Qt_5_6);

       int pasteDataRowCount = 0;
       int pasteDataColumnCount = 0;
       if (!inStream.atEnd())
          inStream >> pasteDataRowCount >> pasteDataColumnCount;
       if (pasteDataRowCount+row > rowCount() || pasteDataColumnCount+column > columnCount())
          return false;

       for (int i=0; i<pasteDataRowCount; i++)
       {
          for (int j=0; j<pasteDataColumnCount && !inStream.atEnd(); j++)
          {
             QVariant data;
             inStream >> data;
             replace(row+i, column+j, data);
          }
       }
       QItemSelection selection(d->feaModel->index(row,column),
                                          d->feaModel->index(row+pasteDataRowCount-1, column+pasteDataColumnCount-1));
       clearSelection();
       selectionChanged(selection,QItemSelection());
    }
    else if (mimeData.hasFormat(GRAPHIC_TYPE))
    {
       QByteArray byteArray = mimeData.data(GRAPHIC_TYPE);
       QDataStream inStream(&byteArray, QIODevice::ReadOnly);
       qint16 graphicType;
       inStream >> graphicType;
       Graphic *newGraphic = createGraphic(graphicType);
       newGraphic -> in(inStream);
       addGraphic(newGraphic);
    }
//    else if (mimeData.hasText())
//    {
//       QString str = mimeData.text();
//       int pasteContentRows = str.count('\n')+1;
//       int pasteContentColumns = str.count('\t')/pasteContentRows + 1;

//       if (row+pasteContentRows > rowCount() || column+pasteContentColumns > columnCount())
//          return false;

//       QStringList sentences = str.split('\n');
//       for (int i=0; i<pasteContentRows; i++)
//       {
//          QStringList words = sentences[i].split('\t');
//          for (int j=0; j<pasteContentColumns; j++)
//          {
//             if (words[j].isEmpty() && !item(row+i, column+j))
//                continue;
//             setItemText(row+i, column+j, words[j]);
//          }
//       }
//       QTableWidgetSelectionRange selectionRange(row, column,
//             row+pasteContentRows-1,
//             column+pasteContentColumns-1);
//       clearSelection();
//       setRangeSelected(selectionRange, true);
//    }
    else
       return false;

    return true;
}

int FeatureWidget::currentRow()
{
     return currentIndex().row();
}

int FeatureWidget::currentColumn()
{
    return currentIndex().column();
}

QVariant FeatureWidget::at(int row, int column, int role) const
{
    if( !d->feaModel ) return QVariant();
    return d->feaModel->data( d->feaModel->index(row,column), role );
}

void FeatureWidget::replace(int row, int column, const QVariant &value, int role)
{
    if( !d->feaModel ) return;
    d->feaModel->setData( d->feaModel->index(row,column), value, role );
}

void FeatureWidget::deleteContents(const QItemSelection &select)
{
    if (select.isEmpty()) return;
    for (int i=select.first().top() ; i<= select.first().bottom(); i++)
    {
       for (int j= select.first().left(); j<=select.first().right(); j++)
       {
            replace(i,j,QVariant(0));
       }
    }
}

QByteArray FeatureWidget::getItemsInByteArray(const QItemSelection &select) const
{
    QByteArray byteArray;
    QDataStream outStream(&byteArray, QIODevice::WriteOnly);
    outStream.setVersion(QDataStream::Qt_5_6);
    outStream << (quint32)0xA0B0C0D0;  // Write a header with a "magic number" and a version

    if(!select.isEmpty())
    {
        outStream << select.first().width() << select.first().height();
        for (int i=select.first().top(); i<=select.first().bottom(); i++)
           for (int j=select.first().right(); j<=select.first().left(); j++)
           {
              outStream << at(i,j);
           }
    }
    return byteArray;
}

QByteArray FeatureWidget::getItemsInByteArray(const int top, const int left, const int bottom, const int right) const
{
    QItemSelection select(d->feaModel->index(top,left),
                                    d->feaModel->index(bottom,right));

    return getItemsInByteArray(select);
}

QVariant FeatureWidget::columnHeader(int column, int role)
{
    if( !d->feaModel ) return QVariant();
    return d->feaModel->headerData( column, Qt::Horizontal, role );
}

QVariantList FeatureWidget::columnHeaders(int role)
{
    if( !d->feaModel ) return QVariantList();
    QVariantList result;
    int count = d->feaModel->columnCount();
    result.reserve(count);
    for( int i = 0; i < count; ++i )
        result[i] = d->feaModel->headerData(i, Qt::Horizontal, role);
    return result;
}

void FeatureWidget::setColumnHeader(int column, QVariant header, int role)
{
    if( !d->feaModel ) return;
    d->feaModel->setHeaderData( column, Qt::Horizontal, header, role );
}

void FeatureWidget::setColumnHeaders(QVariantList list, int role)
{
    for( int i = 0; i < list.size(); ++i )
        setColumnHeader( i, list.at(i), role );
}

QVariantList FeatureWidget::row(int index, int role) const
{
    if( !d->feaModel ) return QVariantList();
    if( ( index < 0 ) || ( index >= d->feaModel->rowCount() ) ) return QVariantList();
    QVariantList result;
    int count = d->feaModel->columnCount();
    result.reserve(count);
    for( int i = 0; i < count; ++i )
        result[i] = d->feaModel->data( d->feaModel->index( index, i ), role );
    return result;
}

QVariantList FeatureWidget::column(int index, int role) const
{
    if( !d->feaModel ) return QVariantList();
    if( ( index < 0 ) || ( index >= d->feaModel->columnCount() ) ) return QVariantList();
    QVariantList result;
    int count = d->feaModel->rowCount();
    result.reserve( count );
    for( int i = 0; i < count; ++i )
        result << d->feaModel->data( d->feaModel->index( i, index ), role );
    return result;
}

int FeatureWidget::rowCount()
{
    return d->feaModel? d->feaModel->rowCount():0;
}

void FeatureWidget::insertRows(int row, int count)
{
    if( !d->feaModel ) return;
    bool nullColumn = d->feaModel->columnCount() == 0;
    d->feaModel->insertRows( row, count);
    if( nullColumn && ( d->feaModel->columnCount() == 1 ) ) d->feaModel->removeColumn(0);
}

void FeatureWidget::insertRows(int row, QList<QVariantList> rows, int role)
{
    if( !d->feaModel ) return;
    if( rows.size() == 0 ) return;
    int columns = d->feaModel->columnCount();
    d->feaModel->insertRows( row, rows.size());
    if( ( columns == 0 ) && ( d->feaModel->columnCount() == 1 ) ) d->feaModel->removeColumn(0);
    columns = qMin( columns, rows[0].size() );
    for( int irow = 0; irow < rows.size(); ++irow )
        for( int icolumn = 0; icolumn < columns; ++icolumn  )
            d->feaModel->setData( d->feaModel->index( row+irow, icolumn), rows[irow].at(icolumn), role );
}

void FeatureWidget::insertRow(int row, QVariantList data, int role)
{
    insertRows( row, QList<QVariantList>() << data, role );
}

void FeatureWidget::appendRows(int count)
{
    insertRows( rowCount(), count );
}

void FeatureWidget::appendRows(QList<QVariantList> rows, int role)
{
    insertRows( rowCount(), rows, role );
}

void FeatureWidget::appendRow(QVariantList row, int role)
{
    insertRows( rowCount(), QList<QVariantList>() << row, role );
}

void FeatureWidget::prependRows(int count)
{
    insertRows( 0, count );
}

void FeatureWidget::prependRows(QList<QVariantList> rows, int role)
{
    insertRows( 0, rows, role );
}

void FeatureWidget::prependRow(QVariantList row, int role)
{
    insertRows( 0, QList<QVariantList>() << row, role );
}

void FeatureWidget::removeRows(int row, int count)
{
    if( !d->feaModel ) return;
    d->feaModel->removeRows( row, count);
}

void FeatureWidget::removeRow(int row)
{
    if( !d->feaModel ) return;
    d->feaModel->removeRows( row, 1);
}

void FeatureWidget::setRows(int row, QList<QVariantList> rows, int role)
{
    if( !d->feaModel ) return;
    if( rows.size() == 0 ) return;
    int rcount = qMin( rows.size(), d->feaModel->rowCount()-row );
    int columns = qMin( rows[0].size(), d->feaModel->columnCount() );
    for( int r = 0; r < rcount; ++r )
        for( int c = 0; c < columns; ++c )
            d->feaModel->setData( d->feaModel->index( row+r, c ), rows[r].at(c), role );
}

void FeatureWidget::setRow(int row, QVariantList data, int role)
{
    setRows( row, QList<QVariantList>() << data, role );
}

void FeatureWidget::replaceRows(int row, QList<QVariantList> rows, int role)
{
    setRows( row, rows, role );
}

void FeatureWidget::replaceRow(int row, QVariantList data, int role)
{
    setRows( row, QList<QVariantList>() << data, role );
}

void FeatureWidget::setRowCount(int count)
{
    if( !d->feaModel ) return;
    int rows = rowCount();
    if( count > rows ) appendRows( count-rows );
    if( count < rows ) removeRows( count, rows-count );
}

int FeatureWidget::columnCount()
{
    return d->feaModel? d->feaModel->columnCount():0;
}

void FeatureWidget::insertColumns(int column, int count)
{
    if( !d->feaModel ) return;
    bool nullRow = d->feaModel->rowCount() == 0;
    d->feaModel->insertColumns( column, count);
    if( nullRow && ( d->feaModel->rowCount() == 1 ) ) d->feaModel->removeRow(0);
}

void FeatureWidget::insertColumns(int column, QList<QVariantList> columns, int role)
{
    if( !d->feaModel ) return;
    if( columns.size() == 0 ) return;
    int rows = d->feaModel->rowCount();
    d->feaModel->insertColumns( column, columns.size() );
    if( ( rows == 0 ) && ( d->feaModel->rowCount() == 1 ) ) d->feaModel->removeRow(0);
    rows = qMin( rows, columns[0].size() );
    for( int icolumn = 0; icolumn < columns.size(); ++icolumn )
        for( int irow = 0; irow < rows; ++irow  )
            d->feaModel->setData( d->feaModel->index( irow, column+icolumn ), columns[icolumn].at(irow), role );
}

void FeatureWidget::insertColumn(int column, QVariantList data, int role)
{
    insertColumns( column, QList<QVariantList>() << data, role );
}

void FeatureWidget::appendColumns(int count)
{
    insertColumns( columnCount(), count );
}

void FeatureWidget::appendColumns(QList<QVariantList> columns, int role)
{
    insertColumns( columnCount(), columns, role );
}

void FeatureWidget::appendColumn(QVariantList column, int role)
{
    insertColumns( columnCount(), QList<QVariantList>() << column, role );
}

void FeatureWidget::prependColumns(int count)
{
    insertColumns( 0, count );
}

void FeatureWidget::prependColumns(QList<QVariantList> columns, int role)
{
    insertColumns( 0, columns, role );
}

void FeatureWidget::prependColumn(QVariantList column, int role)
{
    insertColumns( 0, QList<QVariantList>() << column, role );
}

void FeatureWidget::removeColumns(int column, int count)
{
    if( !d->feaModel ) return;
    d->feaModel->removeColumns( column, count );
}

void FeatureWidget::removeColumn(int column)
{
    if( !d->feaModel ) return;
    d->feaModel->removeColumns( column,1 );
}

void FeatureWidget::setColumns(int column, QList<QVariantList> columns, int role)
{
    if( !d->feaModel ) return;
    if( columns.size() == 0 ) return;
    int ccount = qMin( columns.size(), d->feaModel->columnCount()-column );
    int rows = qMin( columns[0].size(), d->feaModel->rowCount() );
    for( int c = 0; c < ccount; ++c )
        for( int r = 0; r < rows; ++r )
            d->feaModel->setData( d->feaModel->index( r, c+column ), columns[c].at(r), role );
}

void FeatureWidget::setColumn(int column, QVariantList data, int role)
{
    setColumns( column, QList<QVariantList>() << data, role );
}

void FeatureWidget::replaceColumns(int column, QList<QVariantList> columns, int role)
{
    setColumns( column, columns, role );
}

void FeatureWidget::replaceColumn(int column, QVariantList data, int role)
{
    setColumns( column, QList<QVariantList>() << data, role );
}

void FeatureWidget::setColumnCount(int count)
{
    if( !d->feaModel ) return;
    int columns = columnCount();
    if( count > columns ) appendColumns( count-columns );
    if( count < columns ) removeColumns( count, columns-count );
}

Graphic *FeatureWidget::getGraphic(int num) const
{
    if (num >= 0 && num < d->graphics.size())
       return d->graphics[num];
    else
        return 0;
}

Graphic *FeatureWidget::getSelectedGraphic() const
{
    return d->selectedGraphic;
}

QByteArray FeatureWidget::getGraphicInByteArray(const Graphic *curGraphic) const
{
    QByteArray byteArray;
    QDataStream outStream(&byteArray, QIODevice::WriteOnly);
    curGraphic -> out(outStream);
    return byteArray;
}

Graphic *FeatureWidget::createGraphic(int graphicType) const
{
    if (graphicType == ImageType)
       return new PictureGraphic(viewport());
    else
        return 0;
}

int FeatureWidget::getGraphicCount() const
{
   return d->graphics.size();
}

void FeatureWidget::deleteGraphic(int num)
{
    if (num >= 0 && num < d->graphics.size())
       //graphics should not be deleted to support Undo and Redo;
       d->graphics[num] -> hide();
    someThingChanged();
}

void FeatureWidget::deleteGraphic(Graphic *graphicPointer)
{
    for (int i=0; i<d->graphics.size(); i++)
       if (d->graphics[i] == graphicPointer)
          deleteGraphic(i);
    if (d->selectedGraphic == graphicPointer)
        d->selectedGraphic = 0;
}

void FeatureWidget::addGraphic(Graphic *newGraphic)
{
    int i;
    if (!newGraphic)
       return;
    for (i=0; i<d->graphics.size() && d->graphics[i] != newGraphic ; i++);
    if (i == d->graphics.size()) //if newGraphic does not exist:
    {
       d->graphics.append(newGraphic);
       connect(newGraphic, SIGNAL(graphicDraggedSignal(Graphic*, QPoint, QPoint)),
               this, SLOT(graphicDraggedSlot(Graphic*, QPoint, QPoint)));
       connect(newGraphic, SIGNAL(graphicResizedSignal(Graphic*, QSize, QSize,
               Graphic::BoundaryPosition)),
               this, SLOT(graphicResizedSlot(Graphic*,QSize,QSize,Graphic::BoundaryPosition)));
       connect(newGraphic, SIGNAL(graphicSelectedSignal(Graphic*)),
               this, SLOT(graphicSelectedSlot(Graphic*)));
       connect(newGraphic, SIGNAL(graphicContextMenuSignal(QPoint)),
               this, SIGNAL(graphicContextMenuSignal(QPoint)));
       connect(newGraphic, SIGNAL(somethingChangedSignal()),
               this, SLOT(someThingChanged()));
    }
    newGraphic -> show();
    someThingChanged();
}

////QJsonDocument FeatureWidget::json() const
////{
////    return QJsonDocument::fromJson(_features.toDocument().toJson());
////}

void FeatureWidget::propEditPressed()
{
    QDialog infoDialog(this);

    ui.setupUi(&infoDialog);
    Qt::WindowFlags flags = Qt::Tool;
    infoDialog.setWindowFlags(flags);
    ui.idEdit->setText(d->feaModel->id());
    ui.typeEdit->setText(d->feaModel->type());
    ui.treeView->loadJson(QJsonDocument(d->feaModel->properties()).toJson());
    ui.treeView->setVisible(false);

    connect(ui.applyBtn,SIGNAL(clicked()),this,SLOT(updateProp()));

    infoDialog.exec();
}

void FeatureWidget::someThingChanged()
{
    viewport() -> update();
    emit modified();
}

void FeatureWidget::graphicDraggedSlot(Graphic *, QPoint, QPoint)
{
    // to do...
}

void FeatureWidget::graphicResizedSlot(Graphic *, QSize, QSize, Graphic::BoundaryPosition)
{
    // to do...
}

void FeatureWidget::graphicSelectedSlot(Graphic *)
{
    // to do...
}

void FeatureWidget::init()
{
    // 设置表头
    TableHeaderView *hv = new TableHeaderView(Qt::Horizontal, this);
    hv->setSectionsClickable(true);
    hv->setHighlightSections(true);
    this->setHorizontalHeader(hv);

    connect(hv,SIGNAL(changeColumnEditor(int,FeatureWidget::ColumnType)),
                this, SLOT(changeCellEditor(int,FeatureWidget::ColumnType)));

    //把表格的背景调成黄蓝相间,这种方法是在网上看到的，用起来还真方便啊
    setAlternatingRowColors(true);
    setStyleSheet("QTableView{background-color: rgb(250, 250, 115);"
        "alternate-background-color: rgb(141, 163, 215);}");

    verticalHeader()->setVisible(true);
    horizontalHeader()->setStretchLastSection(true);
    setItemDelegate( new ReadOnlyDelegate(this));  // 设置为只读
}

////void FeatureWidget::currentFeatureSelect(int num)
////{
////    if(num != _indexF && _indexF != -1){
////        _features.list.replace(_indexF,d->feaModel->feature());
////    }
////   _indexF = num;
////   d->feaModel->setFeature(_features.list.at(num));
////}


////QString FeatureWidget::importASCII()
////{
////    ImportASCIIDialog *import_dialog = new ImportASCIIDialog(true, this);
//////	import_dialog->setDirectory(asciiDirPath);
//////	import_dialog->selectNameFilter(d_ASCII_file_filter);
////    if (import_dialog->exec() != QDialog::Accepted)
////        return "";

////    if(import_dialog->selectedNameFilter() == tr("GPJson files") + " (*.gpjson *.GPJSON)")  {
////        loadGPjsonFile(import_dialog->selectedFiles().at(0));
////    }
////    else {
////        d->feaModel->importASCII(import_dialog->selectedFiles().at(0),
////                                   import_dialog->importMode(),
////                                   import_dialog->columnSeparator(),
////                                   import_dialog->ignoredLines(),
////                                   import_dialog->renameColumns(),
////                                   import_dialog->stripSpaces(),
////                                   import_dialog->simplifySpaces(),
////                                   import_dialog->convertToNumeric(),
////                                   import_dialog->decimalSeparators());
////    }
////    return import_dialog->selectedFiles().at(0);
////}

////bool FeatureWidget::exportASCII(const QString& fn)
////{
////    QString fileName;
////    if (fn.isEmpty())
////    {
////        fileName = QFileDialog::getSaveFileName(this, tr("Import GPjson From File"),
////                                         QString(), "GPJson files (*.gpjson *.GPJSON );;"
////                                                     "Text files (*.TXT *.txt);;"
////                                                      "Data files (*.DAT *.dat)");
////    }
////    else
////        fileName = fn;

////    QFileInfo fileinfo = QFileInfo(fileName);
////    if (fileinfo.suffix().toLower() == "gpjson")
////        return saveGPjsonFile(fileName);
////    else
////        return d->feaModel->exportASCII(fileName);

////}

void FeatureWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu menu(this);
    QAction *propAction = menu.addAction(tr("Edit Properties"));
    QAction *addColumn = menu.addAction(tr("add column"));
    QAction *addRow = menu.addAction(tr("add row"));
//    QAction *saveJson = menu.addAction(tr("Save GPJson File"));
//    QAction *viewProp = menu.addAction(tr("Properties View "));
//    QAction *initRowsColumns = menu.addAction(tr("initial 100*100 "));

    QAction *res = menu.exec(mapToGlobal(e->pos()));
    if (res == propAction)
    {
        propEditPressed();
    }
    if (res == addColumn)
    {
       d->feaModel->insertColumns(0,1);
    }
    if (res == addRow)
    {
       d->feaModel->insertRows(0,1);
    }
//    menu.addSeparator();
//    if (res == viewProp)
//    {
////         d->feaModel->removeRows(0,10);
//        infoEditPressed();
    //    }
}

void FeatureWidget::mousePressEvent(QMouseEvent *e)
{
    d->selectedGraphic = 0;
    QTableView::mousePressEvent(e);
}

void FeatureWidget::updateProp()
{
    d->feaModel->setID( ui.idEdit->text().toUtf8());
    d->feaModel->setType( ui.typeEdit->text());

    QJsonModel *model = qobject_cast<QJsonModel*>(ui.treeView->model());
    if(!model)
        return;
    d->feaModel->setProperties(model->json().object());
}

void FeatureWidget::updateDelegate(Qt::Orientation orientation, int first, int last)
{
    // to do.. setDelegateForColumn accorrding to column name
    if(orientation == Qt::Horizontal)
    {
        for (int i = first; i <= last; ++i) {
            QVariant val = d->feaModel->headerData(i, orientation, Qt::DisplayRole).toString();
            if (val.toString().toLower() == "id")
            {
                //setItemDelegateForColumn(i, &userIDDelegate);
            }
        }
    }
}
