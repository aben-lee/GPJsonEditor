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

#include "qjsonview.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QDebug>

QJsonView::QJsonView(QWidget *parent) : QTreeView(parent)
{
    _model = new QJsonModel(this);
    setModel(_model);

    //把表格的背景调成黄蓝相间,这种方法是在网上看到的，用起来还真方便啊
    setAlternatingRowColors(true);
    setStyleSheet("QTreeView{background-color: rgb(250, 250, 115);"
            "alternate-background-color: rgb(141, 163, 215);}");
}

bool QJsonView::load(const QString &fileName)
{
    return _model -> load(fileName);
}

bool QJsonView::load(QIODevice *device)
{
    return _model->load(device);
}

bool QJsonView::loadJson(const QByteArray &json)
{
    return _model->loadJson(json);
}

QJsonDocument QJsonView::json() const
{
    return _model->json();
}

void QJsonView::insertJsonPair(const QString &key, const QJsonValue &value)
{
    _model->insertJsonPair(currentIndex(),key,value);
}

void QJsonView::deleteJsonPair(const QModelIndex &id)
{
    _model->deleteJsonPair(id);
}

void QJsonView::contextMenuEvent(QContextMenuEvent *e)
{
    // Show the context sensitive menu
    QMenu menu(this);
    QAction *addItemAction = menu.addAction(tr("Add Json Pair"));
    QAction *deleteItemAction = menu.addAction(tr("Delete Json Pair"));

    QAction *res = menu.exec(mapToGlobal(e->pos()));
    if (res == addItemAction)   {
        bool ok;
        QString text =  QInputDialog::getText(this, tr("Add a Json Key"),
                                             tr("Json Key"), QLineEdit::Normal, QString(), &ok);
      if (ok && !text.isEmpty())
         insertJsonPair(text) ;
    }
    if (res == deleteItemAction)    {
        for(auto id : selectedIndexes())
            deleteJsonPair(id);
    }
}
