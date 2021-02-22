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

#ifndef IDATALOADER_H
#define IDATALOADER_H

#include <QString>
#include <QJsonObject>

class IDataLoader
{
public:
    virtual bool canLoadFile(const QString &fileName) = 0;
    virtual bool load(const QString &fileName) = 0;
    virtual QJsonObject jsonData() const = 0;
    virtual void clear() = 0;
};

#endif // IDATALOADER_H
