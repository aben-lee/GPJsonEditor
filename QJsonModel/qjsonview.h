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

#ifndef QJSONVIEW_H
#define QJSONVIEW_H

#include <QTreeView>
#include "qjsonmodel.h"

class QJsonView : public QTreeView
{
    Q_OBJECT
public:
    QJsonView(QWidget *parent = Q_NULLPTR);

    bool load(const QString& fileName);
    bool load(QIODevice * device);
    bool loadJson(const QByteArray& json);
    QJsonDocument json() const;

    void insertJsonPair( const QString &key, const QJsonValue &value = QJsonValue("") );
    void deleteJsonPair( const QModelIndex &id = QModelIndex());
protected:
    virtual void contextMenuEvent (QContextMenuEvent *e) override;

private:
    QJsonModel *_model;
};

#endif // QJSONVIEW_H
