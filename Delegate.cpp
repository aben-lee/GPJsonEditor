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

#include "Delegate.h"
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>


//编号列，只读委托
//这个方法我还真想不到，呵呵
ReadOnlyDelegate::ReadOnlyDelegate(QObject *parent ) : QStyledItemDelegate(parent)
{

}
QWidget* ReadOnlyDelegate::createEditor(QWidget *, const QStyleOptionViewItem &,
        const QModelIndex &) const{
    return NULL;
}

ReadOnlyDelegate::~ReadOnlyDelegate()
{

}

TextItemDelegate::TextItemDelegate(QObject *parent): QStyledItemDelegate(parent)
{

}

QWidget *TextItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)

    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void TextItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(text);
}

void TextItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString text = lineEdit->text();
    model->setData(index, text, Qt::EditRole);
}

//ID列，只能输入1－12个数字
//利用QLineEdit委托和正则表达式对输入进行限制
UserIDDelegate::UserIDDelegate(QObject *parent): QStyledItemDelegate(parent) { }
UserIDDelegate::~UserIDDelegate(){}
QWidget* UserIDDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
        const QModelIndex &) const
{
    QLineEdit *editor = new QLineEdit(parent);
    QRegExp regExp("[0-9]{0,10}");
    editor->setValidator(new QRegExpValidator(regExp, parent));
    return editor;
}
void UserIDDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(text);
}
void UserIDDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString text = lineEdit->text();
    model->setData(index, text, Qt::EditRole);
}
void UserIDDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

//时间
TimeDelegate::TimeDelegate(QObject *parent):QStyledItemDelegate(parent){

}
TimeDelegate::~TimeDelegate(){}
QWidget* TimeDelegate::createEditor(QWidget* parent , const QStyleOptionViewItem& , const QModelIndex&) const{
    QDateTimeEdit *time = new QDateTimeEdit(QDate::currentDate() , parent);
    time->setDisplayFormat("HH:mm:ss");
    return time;
}
void TimeDelegate::setEditorData(QWidget* editor , const QModelIndex &index)const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
//    printf("value:%s\n" , value.toStdString().c_str());
    QDateTimeEdit *time = static_cast<QDateTimeEdit*>(editor);
    QTime data = QTime::fromString(value);
    time->setTime(data);
}
void TimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const
{
    QDateTimeEdit *time = static_cast<QDateTimeEdit*>(editor);
    QTime data = time->time();
    model->setData(index, data, Qt::EditRole);
}
void TimeDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

//性别列，利用QComboBox委托对输入进行限制
//这一列的单元格只能输入Male或Female
SexDelegate::SexDelegate(QObject *parent ): QStyledItemDelegate(parent) { }
SexDelegate::~SexDelegate(){}
QWidget* SexDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)

    QComboBox *editor = new QComboBox(parent);
    editor->addItem("Female");
    editor->addItem("Male");
    return editor;
}
void SexDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int tindex = comboBox->findText(text);
    comboBox->setCurrentIndex(tindex);
}
void SexDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString text = comboBox->currentText();
    model->setData(index, text, Qt::EditRole);
}
void SexDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(option)
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

//头像列，只是在单元格中央放一张小图而已
IconDelegate::IconDelegate(QObject *parent ): QStyledItemDelegate(parent) { }
IconDelegate::~IconDelegate(){}
void IconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
        const QModelIndex & index ) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)
    Q_UNUSED(painter)
    //show.bmp是在工程目录中的一张图片（其实就是QQ的图标啦，呵呵）
    QPixmap pixmap = QPixmap("show.bmp").scaled(24, 24);
    //->style()->drawItemPixmap(painter, option.rect,  Qt::AlignCenter, QPixmap(pixmap));
}

//spinbox，
SpinBoxDelegate::SpinBoxDelegate(QObject *parent) : QStyledItemDelegate(parent){

}

QWidget *SpinBoxDelegate::createEditor(QWidget *parents ,
        const QStyleOptionViewItem & ,
        const QModelIndex &) const{
    QSpinBox *editor = new QSpinBox(parents);
    editor -> setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor ,
        const QModelIndex &index) const{
    int value = index.model()->data(index, Qt::DisplayRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor ,
        QAbstractItemModel *model ,
        const QModelIndex &index) const{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index , value , Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor ,
        const QStyleOptionViewItem &option ,
        const QModelIndex &) const{
    editor->setGeometry(option.rect);
}

//doubleSpinbox，
DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{

}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
            const QStyleOptionViewItem &option,  const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)

    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);

    editor -> setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    editor -> setSingleStep(0.1);

    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant indexValue = index.model()-> data(index, Qt::DisplayRole);
    QDoubleSpinBox *doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
    doubleSpinBox -> setValue(indexValue.toDouble());
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QDoubleSpinBox *doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
    doubleSpinBox->interpretText();
    double value = doubleSpinBox->value();
    model->setData(index, value , Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(editor)

    editor->setGeometry(option.rect);
}

//////////////////////////////////////////////
/// \brief DateTimeItemDelegate::DateTimeItemDelegate
/// \param parent
///
DateTimeDelegate::DateTimeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

QWidget *DateTimeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)
    QDateTimeEdit *editor = new QDateTimeEdit( parent);
    editor->setDisplayFormat("yyyy-MM-ddThh:mm:ss.zzz");
    return editor;
}

void DateTimeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant indexValue = index.model()-> data(index, Qt::DisplayRole);
    QDateTimeEdit *timeEdit = static_cast<QDateTimeEdit*>(editor);
    QDateTime date = QDateTime::fromString(indexValue.toString(),Qt::ISODateWithMs);
    timeEdit->setDateTime(date);
    return;
}

void DateTimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateTimeEdit *timeEdit = static_cast<QDateTimeEdit*>(editor);
    model->setData(index, timeEdit->text(), Qt::EditRole);
    return;
}



//////
/// \brief JsonItemDelegate::JsonItemDelegate
/// \param parent
///
JsonItemDelegate::JsonItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *JsonItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
   QVariant indexValue = index.model() -> data(index, Qt::DisplayRole);

   if(indexValue.type() == QVariant::Bool){
       QComboBox *editor = new QComboBox(parent);
       editor->addItem("true");
       editor->addItem("false");
       return editor;
    }
    else if(indexValue.type() == QVariant::Double){
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor -> setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
        editor -> setSingleStep(0.1);
        return editor;
    }
    else if(indexValue.type() == QVariant::String){
        QDateTime date;
        if(date.fromString(indexValue.toString(),Qt::ISODate).isValid())
        {
            QDateTimeEdit *editor = new QDateTimeEdit( parent);
            editor->setDisplayFormat("yyyy-MM-ddThh:mm:ss.zzz");
            return editor;
        }
        else{
             return new QLineEdit(parent);
        }
    }
   Q_UNUSED(option)
    return new QLineEdit(parent);
}

void JsonItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant indexValue = index.model()-> data(index, Qt::DisplayRole);

    if(indexValue.type() == QVariant::Bool){
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(indexValue.toString());
        comboBox->setCurrentIndex(tindex);
        return;
     }
     else if (indexValue.type() == QVariant::Double) {
       QDoubleSpinBox *doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
       doubleSpinBox -> setValue(indexValue.toDouble());
       return;
   }
   else if(indexValue.type() == QVariant::String) {
        //! fix me
        //! 改变，采用正则表达式判断是字符串否日期格式

        QDateTime date = QDateTime::fromString(indexValue.toString(),Qt::ISODateWithMs);
       if(date.isValid()) {
            QDateTimeEdit *timeEdit = static_cast<QDateTimeEdit*>(editor);
            timeEdit->setDateTime(date);
            return;
       }
       else {
           QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
           lineEdit->setText(indexValue.toString());
           return;
       }
   }
}

void JsonItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QVariant indexValue = index.model()-> data(index, Qt::DisplayRole);

    if(indexValue.type() == QVariant::Bool){
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        QVariant val;
        if (text == "true") val.setValue(true); else val.setValue(false);
        model->setData(index, val, Qt::EditRole);
        return;
     }
     else if (indexValue.type() == QVariant::Double) {
       QDoubleSpinBox *doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
       doubleSpinBox->interpretText();
       double value = doubleSpinBox->value();
       model->setData(index , value , Qt::EditRole);
       return;
    }
    else if(indexValue.type() == QVariant::String) {
        QDateTime date = QDateTime::fromString(indexValue.toString(),Qt::ISODateWithMs);
        date.fromString(indexValue.toString(),Qt::ISODateWithMs);
       if(date.isValid()) {
           QDateTimeEdit *timeEdit = static_cast<QDateTimeEdit*>(editor);
           model->setData(index, timeEdit->text(), Qt::EditRole);
           return;
       }
       else {
           QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
           QString text = lineEdit->text();
           model->setData(index, text.toDouble(), Qt::EditRole);
           return;
       }
   }
}
