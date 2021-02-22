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

#ifndef IJSONMODELITEM_H
#define IJSONMODELITEM_H

#include <QString>
#include <QVariant>
#include <QJsonValue>

class IJsonModel;
class IJsonModelItem
{
public:
    virtual IJsonModel *model() const = 0;
    virtual IJsonModelItem *parentItem() const = 0;
    virtual int row() const = 0;
    virtual QVariant value() const = 0;
};

Q_DECLARE_INTERFACE(IJsonModelItem, "EIET.IJsonModelItem/1.0.0")

#endif // IJSONMODELITEM_H
