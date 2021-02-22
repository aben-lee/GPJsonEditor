/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef IJSONMODEL_H
#define IJSONMODEL_H

#include <QModelIndex>
#include <QAbstractItemModel>

class IJsonModelItem;
class IJsonModel : public QAbstractItemModel
{
public:
    enum Roles
    {
        KeyRole = Qt::UserRole+1,
        FieldNameRole,
        ValueRole,
        ValueTypeRole,
        ValueTypeNameRole,
        ValueHasStringRepresentation,
        MinRole = KeyRole,
        MaxRole = ValueHasStringRepresentation
    };

    virtual QList<IJsonModelItem*> fetchHeaderItemsInRecords(const QString &fieldName) const = 0;
    virtual QModelIndex index(IJsonModelItem *item, int column=0) const = 0;

    //must be signal
    virtual void jsonDocumentModified() = 0;

protected:
    IJsonModel(QObject *parent=0) : QAbstractItemModel(parent) { }
    ~IJsonModel() { }
};

Q_DECLARE_INTERFACE(IJsonModel, "com.EIET.IJsonModel/1.0.0")

#endif // IJSONMODEL_H
