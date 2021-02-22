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

#include "TreeTableModel.h"
#include "JsonModel_p.h"

struct JsonModelItemData
{
    JsonModel *model;
    JsonModelItem *parent;
    QList<JsonModelItem*> children;
    QString key;
    QVariant value;
    bool expanded;
    int filterCounter;
};

JsonModelItem::JsonModelItem(const QString &key, const QVariant &value, JsonModel *model)
{
    d = new JsonModelItemData;
    d->model = model;
    d->parent = 0;
    d->key = key;
    d->value = value;
    d->expanded = false;
    d->filterCounter = 0;
    d->model->addToTopLevelItems(this);
    d->model->itemAdded(this);
}

JsonModelItem::JsonModelItem(const QString &key, const QVariant &value, JsonModelItem *parent)
{
    d= new JsonModelItemData;
    d->model = dynamic_cast<JsonModel*>(parent->model());
    d->parent = parent;
    d->key = key;
    d->value = value;
    d->expanded = false;
    d->filterCounter = 0;

    if(d->parent)
        d->parent->addTochildList(this);
    d->model->itemAdded(this);
}

JsonModelItem::~JsonModelItem()
{
    if(d->parent)
        d->parent->removeFromChildList(this);
    else
        d->model->removeFromTopLevelItems(this);

    d->parent = 0;
    QList<JsonModelItem*> copy = d->children;
    d->children.clear();
    qDeleteAll(copy);

    d->model->itemRemoved(this);
}

QJsonValue JsonModelItem::jsonValue() const
{
    if(d->value.userType() == QMetaType::QObjectStar)
    {
        TreeTableModel *tableModel = qobject_cast<TreeTableModel*>(d->value.value<QObject*>());
        if(!tableModel)
            return QJsonValue();

        return tableModel->jsonArray();
    }

    if(d->children.size())
    {
        QJsonObject obj;
        Q_FOREACH(JsonModelItem *child, d->children)
            obj[child->key()] = child->jsonValue();

        return obj;
    }

    return QJsonValue::fromVariant(d->value);
}

IJsonModel *JsonModelItem::model() const
{
    return d->model;
}

IJsonModelItem *JsonModelItem::parentItem() const
{
    return static_cast<IJsonModelItem*>(d->parent);
}

JsonModelItem *JsonModelItem::parent() const
{
    return d->parent;
}

int JsonModelItem::row() const
{
    if(d->parent)
        return d->parent->indexOfChildItem(const_cast<JsonModelItem*>(this));
    return d->model->topLevelIndexOf(const_cast<JsonModelItem*>(this));
}

int JsonModelItem::flatRow() const
{
    return d->model->flatListIndexOf( const_cast<JsonModelItem*>(this) );
}

void JsonModelItem::applyFilter(const QString &filter)
{
    int counter = d->filterCounter;
    if( this->fieldName().contains(filter, Qt::CaseInsensitive) )
        ++counter;
    else
        counter = 0;

    if(counter != d->filterCounter)
    {
        d->filterCounter = counter;

        /* Below code was commented because it was consuming too much time
         * calling each items parant's data changed. Now we just reset the model.
         *
        if(counter > 0)
        {
            JsonModelItem *itm = this->parent();
            while(itm)
            {
                ++itm->d->filterCounter;
                d->model->itemDataChanged(itm);
                itm = itm->parent();
            }
        }

        d->model->itemDataChanged(this);
        */
    }
}

int JsonModelItem::depth() const
{
    int ret = -1;
    const JsonModelItem *itm = this;
    while(itm)
    {
        ++ret;
        itm = itm->parent();
    }

    return ret;
}

int JsonModelItem::childCount() const
{
    return d->children.size();
}

JsonModelItem *JsonModelItem::childAt(int index) const
{
    if(index < 0 || index >= d->children.size()) return 0;
    return d->children.at(index);
}

QString JsonModelItem::key() const
{
    if(d->key.isEmpty())
        return QString("[ %1 ]").arg(this->row());
    return d->key;
}

QString JsonModelItem::fieldName(bool includeRoot) const
{
    if(!d->parent)
        return d->key;

    QStringList comps;
    const JsonModelItem *item = this;
    while(item)
    {
        comps.prepend(item->key());
        item = item->parent();
        if(!includeRoot && !item->parent())
            break;
    }

    return comps.join(".");
}

void JsonModelItem::setValue(const QVariant &data)
{
    d->value = data;
    d->model->itemDataChanged(this);
}

QVariant JsonModelItem::value() const
{
    return d->value;
}

int JsonModelItem::valueType() const
{
     return d->value.userType();
}

QString JsonModelItem::valueTypeName() const
{
    return QString::fromLatin1(QMetaType::typeName(d->value.userType()));
}

bool JsonModelItem::canExpand() const
{
     return d->children.size() > 0;
}

bool JsonModelItem::setExpanded(bool val)
{
    if(d->children.isEmpty())
        return false;
    d->expanded = val;
    return true;
}

bool JsonModelItem::isExpanded() const
{
    if(!d->parent)
        return d->expanded;
    return d->parent->isExpanded() ? d->expanded : false;
}

bool JsonModelItem::isVisible() const
{
    if(!d->parent)
        return true;
    if(d->model->filter().isEmpty())
        return d->parent->isExpanded();
    return d->filterCounter > 0;
}

JsonModelItem *JsonModelItem::itemForFieldName(const QString &fieldName) const
{
    QStringList fieldNameParts = fieldName.split('.');

    if(fieldNameParts.count())
    {
        QString fieldNamePart = fieldNameParts.takeFirst();
        if(fieldNamePart == d->key)
        {
            if(fieldNameParts.isEmpty())
                return const_cast<JsonModelItem*>(this);

            Q_FOREACH(JsonModelItem *item, d->children)
            {
                JsonModelItem *itemForFieldName = item->itemForFieldName(fieldNameParts.join("."));
                if(itemForFieldName)
                    return itemForFieldName;
            }
        }
    }

    return 0;
}

void JsonModelItem::addTochildList(JsonModelItem *item)
{
    d->children.append(item);
}

void JsonModelItem::removeFromChildList(JsonModelItem *item)
{
    d->children.removeAll(item);
}

int JsonModelItem::indexOfChildItem(JsonModelItem *item) const
{
    return d->children.indexOf(item);
}

///////////////////////////////////////////////////////////////////////////////

JsonListModel::JsonListModel(JsonModel *parent)
    : QAbstractListModel(parent), m_model(parent),
      m_flatList(m_model->getFlatList())
{
    connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
            SLOT(onSourceDataChanged(QModelIndex,QModelIndex)));
    connect(m_model, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
    connect(m_model, SIGNAL(modelReset()), this, SIGNAL(modelReset()));
    connect(m_model, SIGNAL(jsonDocumentModified()), this, SIGNAL(jsonDocumentModified()));
    connect(m_model, SIGNAL(isJsonDocumentDirtyChanged()), this, SIGNAL(isJsonDocumentDirtyChanged()));
    connect(m_model, SIGNAL(filterChanged()), this, SIGNAL(filterChanged()));
}

void JsonListModel::clear()
{
    m_flatList.clear();
}

void JsonListModel::setExpanded(int row, bool val)
{
    if(row < 0 || row >= m_flatList.size())
        return;

    JsonModelItem *item = m_flatList.at(row);
    if(!item->setExpanded(val))
        return;

    int ir = item->row();
    JsonModelItem *nextItem = 0;
    if(item->parent())
        nextItem = item->parent()->childAt(ir+1);
    else
        nextItem = (ir+1) >= m_model->topLevelItemCount() ? 0 : m_model->topLevelItemAt(ir+1);

    int fromRow = row;
    int toRow = nextItem ? m_flatList.indexOf(nextItem) - 1 : m_flatList.size()-1;
    emit dataChanged(this->index(fromRow,0), this->index(toRow,this->columnCount()));
}

QVariant JsonListModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
        role = PairRole;

    if(role >= DepthRole)
    {
        IJsonModelItem *iJsonModelItem = static_cast<IJsonModelItem*>(index.internalPointer());
        JsonModelItem *item = dynamic_cast<JsonModelItem*>(iJsonModelItem);
        switch(role)
        {
        case DepthRole: return item ? item->depth() : -1;
        case PairRole: return item ? QString("[%3] %1 = %2")
                                     .arg(item->key())
                                     .arg(item->value().toString())
                                     .arg(item->flatRow()) : QString();
        case CanExpandRole: return item ? item->canExpand() : false;
        case IsExpandedRole: return item ? item->isExpanded() : true;
        case IsVisibleRole: return item ? item->isVisible() : false;
        case IsFirstChildRole: return item ? item->row() == 0 : false;
        default: break;
        }
    }

    return m_model->data(this->mapToSource(index), role);
}

QHash<int,QByteArray> JsonListModel::roleNames() const
{
    static QHash<int,QByteArray> roles;
    if(roles.isEmpty())
    {
        roles = m_model->roleNames();
        roles[DepthRole] = "depth";
        roles[PairRole] = "pair";
        roles[CanExpandRole] = "canExpand";
        roles[IsExpandedRole] = "isExpanded";
        roles[IsVisibleRole] = "isVisible";
        roles[IsFirstChildRole] = "isFirstChild";
    }

    return roles;
}

QModelIndex JsonListModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if(!sourceIndex.isValid())
        return QModelIndex();

    IJsonModelItem *item = static_cast<IJsonModelItem*>(sourceIndex.internalPointer());
    JsonModelItem *jsonItem = dynamic_cast<JsonModelItem*>(item);
    int row = m_flatList.indexOf(jsonItem);
    return this->createIndex(row, sourceIndex.column(), item);
}

QModelIndex JsonListModel::mapToSource(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    IJsonModelItem *item = static_cast<IJsonModelItem*>(index.internalPointer());
    return m_model->index(item, index.column());
}
