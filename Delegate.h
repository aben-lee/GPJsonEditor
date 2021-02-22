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

#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

//!*****************************************************************************************
//! TableView 代理列
//!  使用方法
//! 姓名 岁数  体重
//! 小明  18  50.5
//! 小萌  19  55.8
//! 小日  17  60.0
//! 小月  16  55.1
//! 小草  18  58.6
//! int main(int argc, char *argv[])
//! {
//!     QApplication app(argc, argv);
//!
//!     VIPModel *model = new VIPModel(5, 5);
//!     QTableView *tableView = new QTableView;
//!
//!     //把表格的背景调成黄蓝相间
//!     //这种方法是在网上看到的，用起来还真方便啊
//!     tableView->setAlternatingRowColors(true);
//!     tableView->setStyleSheet("QTableView{background-color: rgb(250, 250, 115);"
//!         "alternate-background-color: rgb(141, 163, 215);}");
//!
//!     tableView->setWindowTitle("VIP List");
//!     tableView->resize(700, 400);
//!     tableView->setModel(model);
//!     QStringList headerList;
//!     headerList << "No." << "ID" << "Name" << "Age" << "Sex" << "Show";
//!     model->setHorizontalHeaderLabels(headerList);
//!     tableView->verticalHeader()->setVisible(false);
//!     tableView->horizontalHeader()->setStretchLastSection(true);
//!
//!     //为每一列加载委托
//!     ReadOnlyDelegate readonlydelegate;
//!     tableView->setItemDelegateForColumn(0, &readonlydelegate);
//!     UserIDDelegate userIDDelegate;
//!     tableView->setItemDelegateForColumn(1, &userIDDelegate);
//!     AgeDelegate spinBoxDelegate;
//!     tableView->setItemDelegateForColumn(3, &spinBoxDelegate);
//!     SexDelegate comboBoxDelegate;
//!     tableView->setItemDelegateForColumn(4, &comboBoxDelegate);
//!     IconDelegate iconDelegate;
//!     tableView->setItemDelegateForColumn(5, &iconDelegate);
//!
//!     for(int i=0; i<10; i++)
//!     {
//!         QModelIndex index = model->index(i, i, QModelIndex());
//!         model->setData(index, i);
//!     }
//!
//!     tableView->show();
//!     return app.exec();
//! }
//! ***************************************************************************************


//编号列，只读委托
//这个方法我还真想不到，呵呵
class ReadOnlyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ReadOnlyDelegate(QObject *parent = nullptr);
    virtual QWidget *createEditor(QWidget *, const QStyleOptionViewItem &,
            const QModelIndex &) const;

    ~ReadOnlyDelegate();

};

//ID列，只能输入1－12个数字
//利用QLineEdit委托和正则表达式对输入进行限制
class UserIDDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    UserIDDelegate(QObject *parent = 0);
    ~UserIDDelegate();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
        const QModelIndex &) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const ;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const  ;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &) const  ;
};

//! Item delegate for TableView
class TextItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
        TextItemDelegate(QObject * parent = 0);

        QWidget *createEditor(QWidget *parent , const QStyleOptionViewItem &option ,
                const QModelIndex &index) const;
        void setEditorData(QWidget *editor , const QModelIndex &index) const;
        void setModelData(QWidget *editor , QAbstractItemModel *model ,
                const QModelIndex &index) const;
};

//性别列，利用QComboBox委托对输入进行限制
//这一列的单元格只能输入Male或Female
class SexDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SexDelegate(QObject *parent = 0);
    ~SexDelegate();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const  ;
    void setEditorData(QWidget *editor, const QModelIndex &index) const  ;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const  ;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const  ;
};

//头像列，只是在单元格中央放一张小图而已
class IconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    IconDelegate(QObject *parent = 0);
    ~IconDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
        const QModelIndex & index ) const  ;
};

class DoubleSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    public:
        DoubleSpinBoxDelegate(QObject *parent = 0);

        QWidget *createEditor(QWidget *parent , const QStyleOptionViewItem &option ,
                const QModelIndex &index) const;

        void setEditorData(QWidget *editor , const QModelIndex &index) const;

        void setModelData(QWidget *editor , QAbstractItemModel *model ,
                const QModelIndex &index) const;

        void updateEditorGeometry(QWidget *editor ,
                const QStyleOptionViewItem &option ,
                const QModelIndex &index) const;

};

class SpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    public:
        SpinBoxDelegate(QObject *parent = 0);

        QWidget *createEditor(QWidget *parent , const QStyleOptionViewItem &option ,
                const QModelIndex &index) const;

        void setEditorData(QWidget *editor , const QModelIndex &index) const;

        void setModelData(QWidget *editor , QAbstractItemModel *model ,
                const QModelIndex &index) const;

        void updateEditorGeometry(QWidget *editor ,
                const QStyleOptionViewItem &option ,
                const QModelIndex &index) const;

};

//! Item delegate for TableView
class DateTimeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
        DateTimeDelegate(QObject * parent = 0);

        QWidget *createEditor(QWidget *parent , const QStyleOptionViewItem &option ,
                const QModelIndex &index) const;
        void setEditorData(QWidget *editor , const QModelIndex &index) const;
        void setModelData(QWidget *editor , QAbstractItemModel *model ,
                const QModelIndex &index) const;
};

// 时间
class TimeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TimeDelegate(QObject *parent = 0);
    ~TimeDelegate();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
        const QModelIndex &) const  ;
    void setEditorData(QWidget *editor, const QModelIndex &index) const  ;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const  ;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &) const  ;
};

//! Item delegate for TableView
class JsonItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
        JsonItemDelegate(QObject * parent = 0);

        QWidget *createEditor(QWidget *parent , const QStyleOptionViewItem &option ,
                const QModelIndex &index) const;
        void setEditorData(QWidget *editor , const QModelIndex &index) const;
        void setModelData(QWidget *editor , QAbstractItemModel *model ,
                const QModelIndex &index) const;
};


#endif // DELEGATE_H
