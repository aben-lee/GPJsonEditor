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

#ifndef FEATUREVIEW_H
#define FEATUREVIEW_H

#include <QJsonModel/qjsonmodel.h>
#include <QJsonModel/qjsonview.h>
#include <QJsonObject>
#include <QToolBar>
#include <QDialog>
#include <QMimeData>
#include <QTableView>
#include <QHeaderView>
#include <QUndoStack>
#include <QItemSelectionRange>
#include "ui_FeatureInfo.h"
#include "FeatureModel.h"
#include "gpjson.hpp"
#include "Graphic.h"

#define GRAPHIC_TYPE "Graphic"
#define CELL_TYPE    "Cell"

class TableHeaderView;
class Graphic;

struct FeatureWidgetData;
class FeatureWidget : public QTableView
{
    Q_OBJECT

public:
    enum ColumnType {
            ReadOnlyType = 0,
             TextLineType,
             IntegerType,
             DecimalType,
             DateTimeType,
             DateType,
             TimeType,
             BoolType
       };

    FeatureWidget(QWidget *parent = nullptr);
    FeatureWidget(QSharedPointer<GPJson::Feature> feature, QWidget *parent = nullptr);
        ~FeatureWidget();

    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly )
    bool readOnly() const;
    void setReadOnly(const bool state );

     Q_PROPERTY(QString type READ type /*WRITE setType NOTIFY typeChanged*/)
     QString type() const;
     void setType(const QString &t); // 指定Table 类型{Sheet,Binary,URI,Matrix}
     Q_SIGNAL void typeChanged();

     Q_PROPERTY(QString id READ type WRITE setID NOTIFY idChanged)
     QString id() const;
     void setID(const QString &bytes);
     Q_SIGNAL void idChanged();

     Q_PROPERTY(QJsonObject properties READ properties WRITE setProperties NOTIFY propertiesChanged)
     QJsonObject properties() const;
     void setProperties(const QJsonObject prop);
     Q_SIGNAL void propertiesChanged();

     QSharedPointer<GPJson::Feature> feature();
     void setFeature(GPJson::Feature *fe);
     bool setFeature(const QByteArray &json, QString &errorInfo);// avoid using parse json, but GPjson:: method

//    void newSheet(const int rowCount = 10, const int columnCount = 10,const GPJson::DatasetType &type = GPJson::Sheet);
//    bool writeFeature(const QString &fileName);
//    bool readFeature(const QString &fileName);

//    void resetFeatureCollect();
//    QString importASCII();
//    bool exportASCII(const QString& fn = QString());

//    bool loadGPjsonFile(const QString& fn = QString());
//    bool loadGPjson(QIODevice *device);
//    bool loadGPjson(const QByteArray& json);
//    bool saveGPjsonFile(const QString& fn = QString());
    bool saveGPjson(QIODevice *device);

    void printReview();
    void print();
    void print(const QString& fileName);
    void exportPDF(const QString& fileName);

    QUndoStack *getUndoStack() const;

public slots:
    QItemSelection getSelected();

    void cut(QMimeData &mimeData); //cut data to a QMimeData
    void copy(QMimeData &mimeData); //copy data to a QMimeData
    bool paste(const QMimeData &mimeData, int row, int column); //paste data from a QMimeData, at row, column

    void changeCellEditor(int column, FeatureWidget::ColumnType type);

    void setCurrentCell(const int row, const int column);
    void selectCurrentRow();
    void selectCurrentColumn();

    void findNext(QString&, Qt::CaseSensitivity);
    void findPrevious(QString&, Qt::CaseSensitivity);

public:
    enum {InsertAfter, InsertBefore};
    enum {FindInFormula, FindInDisplayText};
    enum {EditorRole = 32};
    enum {TextLineEditor=0, DateEditor, TimeEditor, IntegerEditor, DecimalEditor};
    enum {ImageType};

    int currentRow();
    int currentColumn();
    QVariant at ( int row, int column, int role = Qt::DisplayRole ) const;
    void replace ( int row, int column, const QVariant & value, int role = Qt::EditRole );
    void deleteContents(const QItemSelection &select);

    QByteArray getItemsInByteArray(const QItemSelection &select) const;
    QByteArray getItemsInByteArray(const int top, const int left, const int bottom, const int right) const;

//    // Overloadings
    QVariant columnHeader( int column, int role = Qt::DisplayRole );
    QVariantList columnHeaders(  int role = Qt::DisplayRole );
    void setColumnHeader( int column, QVariant header, int role = Qt::EditRole );
    void setColumnHeaders( QVariantList list, int role = Qt::EditRole );

    QVariantList row( int index, int role = Qt::DisplayRole ) const;
    QVariantList column( int index, int role = Qt::DisplayRole ) const;

    int rowCount();
    void insertRows( int row, int count );
    void insertRows( int row, QList<QVariantList> rows, int role = Qt::EditRole );
    void insertRow( int row, QVariantList data, int role = Qt::EditRole );
    void appendRows( int count );
    void appendRows( QList<QVariantList> rows, int role = Qt::EditRole );
    void appendRow( QVariantList row, int role = Qt::EditRole );
    void prependRows( int count );
    void prependRows( QList<QVariantList> rows, int role = Qt::EditRole );
    void prependRow( QVariantList row, int role = Qt::EditRole );
    void removeRows( int row, int count );
    void removeRow( int row );
    void setRows( int row, QList<QVariantList> rows, int role = Qt::EditRole );
    void setRow( int row, QVariantList data, int role = Qt::EditRole );
    void replaceRows( int row, QList<QVariantList> rows, int role = Qt::EditRole );
    void replaceRow( int row, QVariantList data, int role = Qt::EditRole );
    void setRowCount( int count );

    int columnCount();
    void insertColumns( int column, int count );
    void insertColumns( int column, QList<QVariantList> columns, int role = Qt::EditRole );
    void insertColumn( int column, QVariantList data, int role = Qt::EditRole );
    void appendColumns( int count );
    void appendColumns( QList<QVariantList> columns, int role = Qt::EditRole );
    void appendColumn( QVariantList column, int role = Qt::EditRole );
    void prependColumns( int count );
    void prependColumns( QList<QVariantList> columns, int role = Qt::EditRole );
    void prependColumn( QVariantList column, int role = Qt::EditRole );
    void removeColumns( int column, int count );
    void removeColumn( int column );
    void setColumns( int column, QList<QVariantList> columns, int role = Qt::EditRole );
    void setColumn( int column, QVariantList data, int role = Qt::EditRole );
    void replaceColumns( int column, QList<QVariantList> columns, int role = Qt::EditRole );
    void replaceColumn( int column, QVariantList data, int role = Qt::EditRole );
    void setColumnCount( int count );

    // the following refer to Qsimplesheet 0.3.9
    Graphic *getGraphic(int) const;
    Graphic *getSelectedGraphic() const;
    QByteArray getGraphicInByteArray(const Graphic*) const;
    Graphic *createGraphic(int) const;
    int getGraphicCount() const;
    void deleteGraphic(int);
    void deleteGraphic(Graphic*);
    void addGraphic(Graphic*);

signals:
    void userChangedItemDataSignal(int, int, QString&, QString&);
    void graphicContextMenuSignal(QPoint);
      void modified();

protected:
    void contextMenuEvent (QContextMenuEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;

private slots:
    void updateProp();
    void updateDelegate(Qt::Orientation orientation, int first, int last);
    void propEditPressed();

private slots:
   void someThingChanged();
   void graphicDraggedSlot(Graphic*, QPoint, QPoint);
   void graphicResizedSlot(Graphic*, QSize, QSize, Graphic::BoundaryPosition);
   void graphicSelectedSlot(Graphic*);

private:
    void init();

    FeatureWidgetData *d;
    Ui::FeatureInfo ui;
    friend class TableHeaderView;
};

/*!
    \class refer to QSpreadsheetHeaderView
    \brief The QSpreadsheetHeaderView class is a special QHeaderView that mimic Google Spreadsheet header.
    version 1.0
    \sa QHeaderView
*/
class TableHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    TableHeaderView(Qt::Orientation orientation, QWidget * parent = 0);
        ~TableHeaderView();

signals :
    void changeColumnEditor(int column,  FeatureWidget::ColumnType type);

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

    QRect sectionRect(int logicalIndex) const;
    QRect buttonMenuRect(int logicalIndex) const;
    QRect prevRect(int logicalIndex) const;
    QRect nextRect(int logicalIndex) const;

    void drawMenuButton(QPainter *painter, int logicalIndex, bool enabled) const;
    void drawPrevButton(QPainter *painter, int logicalIndex) const;
    void drawNextButton(QPainter *painter, int logicalIndex) const;

};

#endif // FEATUREVIEW_H
