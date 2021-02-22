#include "TreeTableModel.h"
#include "JsonModel_p.h"
#include "qmath.h"
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QDateTime>

struct JsonModelData
{
    JsonModelData() : listModel(0){}

    QList<QAbstractTableModel *> jsonTableModelList;
    QList<JsonModelItem*> topLevelItems;
    QList<JsonModelItem*> flatList;
    JsonListModel *listModel;
    QJsonDocument jsonDocument;
    bool inSetDocument;
    bool isJsonDocumentDirty;
    QString filter;
};

JsonModel::JsonModel(QObject *parent) : IJsonModel(parent)
{
    d = new JsonModelData;
    d->listModel = new JsonListModel(this);
    d->inSetDocument = false;
    d->isJsonDocumentDirty = false;
    connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(setJsonDocumentDirty()));
}

JsonModel::JsonModel(QJsonDocument doc, QObject *parent) : IJsonModel(parent)
{
    d = new JsonModelData;
    d->listModel = new JsonListModel(this);
    d->inSetDocument = false;
    d->isJsonDocumentDirty = false;
    d->jsonDocument = doc;
    connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(setJsonDocumentDirty()));
}

JsonModel::~JsonModel()
{
    QList<JsonModelItem*> copy = d->topLevelItems;
    d->topLevelItems.clear();
    qDeleteAll(copy);
    qDeleteAll(d->jsonTableModelList);
    d->jsonTableModelList.clear();
    delete d;
}

void JsonModel::setJsonDocumentFromFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
        return;

    qApp->setOverrideCursor(Qt::WaitCursor);
    clear();
    const QByteArray bytes = file.readAll();
    this->setJsonDocumentFromByteArray(bytes);
    qApp->restoreOverrideCursor();
}

void JsonModel::clear()
{
    this->beginResetModel();
    this->blockSignals(true);
    QList<JsonModelItem*> copy = d->topLevelItems;
    d->topLevelItems.clear();
    d->listModel->clear();
    qDeleteAll(copy);
    qDeleteAll(d->jsonTableModelList);
    d->jsonTableModelList.clear();

    this->blockSignals(false);
    this->endResetModel();
}

void JsonModel::setJsonDocument(const QJsonDocument &jsonDoc)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    this->beginResetModel();
    clear();

    d->jsonDocument = jsonDoc;
    const QJsonObject object = jsonDoc.object();
    d->listModel->blockSignals(true);
    d->inSetDocument = true;

    this->load(object, 0);

    d->inSetDocument = false;
    d->isJsonDocumentDirty = false;
    d->listModel->blockSignals(false);

    emit jsonDocumentModified();
    this->endResetModel();
    qApp->restoreOverrideCursor();
}

QJsonDocument JsonModel::jsonDocument() const
{
    return d->jsonDocument;
}

bool JsonModel::isJsonDocumentDirty() const
{
    return d->isJsonDocumentDirty;
}

void JsonModel::setFilter(const QString &filter)
{
    const QString filter2 = filter.simplified();
    if(d->filter == filter2)
        return;

    this->beginResetModel();

    d->filter = filter2;
    Q_FOREACH(JsonModelItem *item, d->flatList)
        item->applyFilter(filter);

    this->endResetModel();

    emit filterChanged();
}

QString JsonModel::filter() const
{
     return d->filter;
}

void JsonModel::reconstructJsonDocument()
{
    QJsonObject jsonObject;
    Q_FOREACH(JsonModelItem *tlItem, d->topLevelItems)
        jsonObject[tlItem->key()] = tlItem->jsonValue();

    d->jsonDocument = QJsonDocument(jsonObject);
    d->isJsonDocumentDirty = false;
    emit isJsonDocumentDirtyChanged();
    emit jsonDocumentModified();
}

JsonListModel *JsonModel::listModel() const
{
    return d->listModel;
}

QObject *JsonModel::tableModel(int index) const
{
    return (QObject*)d->jsonTableModelList.at(index);
}

int JsonModel::tableModelListCount() const
{
    return d->jsonTableModelList.count();
}

QObject *JsonModel::listModelObject()
{
    return reinterpret_cast<QObject*>(d->listModel);
}

QString JsonModel::xyvSurferData() const
{
    QString xyvData;
    QList<JsonModelItem*> topLevelItemsList = topLevelItems();

    Q_FOREACH(JsonModelItem *item, topLevelItemsList)
    {
        QString searchField = item->key() + ".Header.";

        double xLocation = 0;
        if(item->itemForFieldName(searchField + "COIL_LOCATION.X"))
            xLocation = item->itemForFieldName(searchField + "COIL_LOCATION.X")->value().toDouble();

        QVariant data = item->itemForFieldName(item->key() + ".Data")->value();

        TreeTableModel *model = 0;
        if (data.canConvert<TreeTableModel*>())
            model = data.value<TreeTableModel*>();

        if(model)
        {
            for(int i=0; i<model->rowCount(QModelIndex()); i++)
            {
                QVariant depthShiftValue = model->value(i, "TIME/DEPTH");
                QVariant resistivityValue = model->value(i, "RESISTIVITY");
                xyvData.append(QString::number(xLocation) + " " +
                               depthShiftValue.toString() + " " +
                               resistivityValue.toString() + "\n");
            }
        }
    }

    return xyvData;
}

QString JsonModel::xyzvSurferData() const
{
    QString xyzvData;
    QList<JsonModelItem*> topLevelItemsList = topLevelItems();

    Q_FOREACH(JsonModelItem *item, topLevelItemsList)
    {
        QString searchField = item->key() + ".Header.";

        double xLocation = 0;
        if(item->itemForFieldName(searchField + "COIL_LOCATION.X"))
            xLocation = item->itemForFieldName(searchField + "COIL_LOCATION.X")->value().toDouble();

        double yLocation = 0;
        if(item->itemForFieldName(searchField + "COIL_LOCATION.Y"))
            yLocation = item->itemForFieldName(searchField + "COIL_LOCATION.Y")->value().toDouble();

        QVariant data = item->itemForFieldName(item->key() + ".Data")->value();

        TreeTableModel *model = 0;
        if (data.canConvert<TreeTableModel*>())
            model = data.value<TreeTableModel*>();

        if(model)
        {
            for(int i=0; i<model->rowCount(QModelIndex()); i++)
            {
                QVariant depthShiftValue = model->value(i, "TIME/DEPTH");
                QVariant resistivityValue = model->value(i, "RESISTIVITY");
                xyzvData.append(QString::number(xLocation) + " " +
                                QString::number(yLocation) + " " +
                               depthShiftValue.toString() + " " +
                               resistivityValue.toString() + "\n");
            }
        }
    }

    return xyzvData;
}

QString JsonModel::surferGridData() const
{
    QString surferGridData;

    QList<JsonModelItem*> topLevelItemsList = topLevelItems();

    if(!topLevelItemsList.count())
        return QString();

    Q_FOREACH(JsonModelItem *item, topLevelItemsList)
    {
        QString searchField = item->key() + ".Header.";

        double xLocation = 0;
        if(item->itemForFieldName(searchField + "COIL_LOCATION.X"))
            xLocation = item->itemForFieldName(searchField + "COIL_LOCATION.X")->value().toDouble();

        QVariant data = item->itemForFieldName(item->key() + ".Data")->value();

        TreeTableModel *model = 0;
        if(data.canConvert<TreeTableModel*>())
            model = data.value<TreeTableModel*>();

        if(model)
        {
            for(int i=0; i<model->rowCount(QModelIndex()); i++)
            {
                QVariant depthShiftValue = model->value(i, "TIME/DEPTH");
                QVariant resistivityValue = model->value(i, "RESISTIVITY");

                surferGridData.append(QString::number(xLocation) + " ");
                surferGridData.append(depthShiftValue.toString() + " ");
                surferGridData.append(resistivityValue.toString() + "\n");
            }
        }
    }

    return surferGridData;
}

int JsonModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        IJsonModelItem *iJsonModelItem= static_cast<IJsonModelItem*>(parent.internalPointer());
        JsonModelItem* parentItem  = dynamic_cast<JsonModelItem*>(iJsonModelItem);
        return parentItem->childCount();
    }

    return d->topLevelItems.size();
}

int JsonModel::columnCount(const QModelIndex &/*parent*/) const
{
    return (IJsonModel::MaxRole-IJsonModel::MinRole)+1;
}

QModelIndex JsonModel::index(int row, int column, const QModelIndex &parent) const
{
    IJsonModelItem *iJsonModelItem = parent.isValid() ? static_cast<JsonModelItem*>(parent.internalPointer()) : 0;
    JsonModelItem* parentItem  = dynamic_cast<JsonModelItem*>(iJsonModelItem);
    JsonModelItem *item = 0;
    try {
        item = parentItem ? parentItem->childAt(row) : d->topLevelItems.at(row);
    } catch(const std::exception &) {
        return QModelIndex();
    }
    return this->createIndex(row, column, static_cast<IJsonModelItem*>(item));
}

QModelIndex JsonModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    IJsonModelItem *item = static_cast<IJsonModelItem*>(child.internalPointer());
    IJsonModelItem *parentItem = item ? item->parentItem() : 0;
    if(parentItem)
        return this->createIndex(parentItem->row(), 0, static_cast<IJsonModelItem*>(parentItem));

    return QModelIndex();
}

QVariant JsonModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    IJsonModelItem *iJsonModelItem = static_cast<IJsonModelItem*>(index.internalPointer());
    JsonModelItem* item  = dynamic_cast<JsonModelItem*>(iJsonModelItem);
    if(!item)
        return QVariant();

    if(role == Qt::DisplayRole)
        role = index.column() + IJsonModel::MinRole;

    switch(role)
    {
    case IJsonModel::KeyRole: return item->key();
    case IJsonModel::FieldNameRole: return item->fieldName();
    case IJsonModel::ValueRole: return item->value();
    case IJsonModel::ValueTypeRole: return item->valueType();
    case IJsonModel::ValueTypeNameRole: {
        if( item->value().userType() == QMetaType::QObjectStar ) {
            QObject *object = item->value().value<QObject*>();
            return QLatin1String(object->metaObject()->className());
        }
        return QLatin1String( QMetaType::typeName(item->value().userType()) );
    }
    case IJsonModel::ValueHasStringRepresentation: return item->value().canConvert(QMetaType::QString);
    default: break;
    }

    return QVariant();
}

bool JsonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || role != IJsonModel::ValueRole)
        return false;

    JsonModelItem *iJsonModelItem = static_cast<JsonModelItem*>(index.internalPointer());
    JsonModelItem* item  = dynamic_cast<JsonModelItem*>(iJsonModelItem);

    if(!item)
        return false;

    item->setValue(value);
    return true;
}

Qt::ItemFlags JsonModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QHash<int,QByteArray> JsonModel::roleNames() const
{
    static QHash<int,QByteArray> roles;
    if(roles.size() == 0)
    {
        roles[IJsonModel::KeyRole] = "key";
        roles[IJsonModel::FieldNameRole] = "fieldName";
        roles[IJsonModel::ValueRole] = "value";
        roles[IJsonModel::ValueTypeRole] = "valueType";
        roles[IJsonModel::ValueTypeNameRole] = "valueTypeName";
        roles[IJsonModel::ValueHasStringRepresentation] = "valueHasStringRepresentation";
    }
    return roles;
}

void JsonModel::addToTopLevelItems(JsonModelItem *item)
{
    d->topLevelItems.append(item);
}

void JsonModel::removeFromTopLevelItems(JsonModelItem *item)
{
    d->topLevelItems.removeAll(item);
}

int JsonModel::topLevelItemCount()
{
    return d->topLevelItems.count();
}

JsonModelItem *JsonModel::topLevelItemAt(int index)
{
    if(d->topLevelItems.count() <= index)
        return NULL;

    return d->topLevelItems.at(index);
}

int JsonModel::topLevelIndexOf(JsonModelItem *item)
{
    return d->topLevelItems.indexOf(item);
}

QList<JsonModelItem *>& JsonModel::getFlatList()
{
    return d->flatList;
}

int JsonModel::flatListIndexOf(JsonModelItem* item)
{
    return d->flatList.indexOf(item);
}

QList<IJsonModelItem *> JsonModel::fetchHeaderItemsInRecords(const QString &fieldName) const
{
    QList<IJsonModelItem*> items;

    Q_FOREACH(JsonModelItem* item, d->topLevelItems)
    {
        QString searchField = item->key() + ".Header.";

        JsonModelItem* fieldItem = item->itemForFieldName(searchField + fieldName);

        if(fieldItem)
            items.append(static_cast<IJsonModelItem*>(fieldItem));
    }

    return items;
}

QModelIndex JsonModel::index(IJsonModelItem *item, int column) const
{
    if(!item)
        return QModelIndex();

    return this->createIndex(item->row(), column, static_cast<IJsonModelItem*>(item));
}

void JsonModel::itemDataChanged(JsonModelItem *item)
{
    const QModelIndex start = this->index(item, 0);
    const QModelIndex end = this->index(item, this->columnCount(QModelIndex()));
    emit dataChanged(start, end);
}

void JsonModel::load(const QJsonValue &value, JsonModelItem *parent)
{
    if(value.type() == QJsonValue::Object)
    {
        const QJsonObject object = value.toObject();
        QJsonObject::const_iterator it = object.constBegin();
        QJsonObject::const_iterator end = object.constEnd();
        while(it != end)
        {
            const QString key = it.key();
            const QJsonValue value = it.value();
            QVariant variant;
            if(key == "Data" && parent)
                variant = this->toVariant(parent->key(), key, value);
            else
                variant = this->toVariant("Root", key, value);

            JsonModelItem *item = 0;
            if(parent)
                item = new JsonModelItem(key, variant, parent);
            else
                item = new JsonModelItem(key, variant, this);

            if(this->hasChildItems(key, value))
                this->load(value, item);

            ++it;
        }
    }
    else if(value.type() == QJsonValue::Array)
    {
        const QJsonArray array = value.toArray();
        QJsonArray::const_iterator it = array.constBegin();
        QJsonArray::const_iterator end = array.constEnd();
        while(it != end)
        {
            const QString key;
            const QJsonValue value = *it;
            QVariant variant;
            if(key == "Data" && parent)
                variant = this->toVariant(parent->key(), key, value);
            else
                variant = this->toVariant("Root", key, value);

            JsonModelItem *item = 0;
            if(parent)
                item = new JsonModelItem(key, variant, parent);
            else
                item = new JsonModelItem(key, variant, this);

            if(this->hasChildItems(key, value))
                this->load(value, item);

            ++it;
        }
    }
}

void JsonModel::itemAdded(JsonModelItem *item)
{
    // FIXME: What happens when a new row is added after a JSON document
    // is fully loaded?
    int frow = d->inSetDocument ? d->flatList.size() : -1;

    d->listModel->beginInsertRows(QModelIndex(), frow, frow);
    if(frow >= d->flatList.size())
        d->flatList.append(item);
    else
        d->flatList.insert(frow, item);
    d->listModel->endInsertRows();
}

void JsonModel::itemRemoved(JsonModelItem *item)
{
    int frow = d->flatList.indexOf(item);
    if(frow < 0)
        return;

    d->listModel->beginRemoveRows(QModelIndex(), frow, frow);
    d->flatList.removeAt(frow);
    d->listModel->endRemoveRows();
}

QVariant JsonModel::toVariant(const QString &parentKey, const QString &key, const QJsonValue &value)
{
    Q_UNUSED(key)

    if(value.type() == QJsonValue::Array)
    {
        TreeTableModel *tableModel = new TreeTableModel(const_cast<JsonModel*>(this));

        tableModel->setRecordName(parentKey);
//        tableModel->addRows(value.toArray());

        d->jsonTableModelList.append((QAbstractTableModel*)tableModel);
        connect(tableModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(setJsonDocumentDirty()));
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(setJsonDocumentDirty()));
        connect(tableModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(setJsonDocumentDirty()));
        return QVariant::fromValue<QObject*>( (QObject*)tableModel );
    }

    if(value.type() == QJsonValue::Object)
        return QVariant();

    return value.toVariant();
}

QJsonValue JsonModel::fromVariant(const QString &key, const QVariant &value) const
{
    Q_UNUSED(key);
    Q_UNUSED(value);
    return QJsonValue();
}

bool JsonModel::hasChildItems(const QString &key, const QJsonValue &value) const
{
    Q_UNUSED(key);
    return (value.type() == QJsonValue::Object);
}

void JsonModel::setJsonDocumentDirty()
{
    if(!d->isJsonDocumentDirty) {
        d->isJsonDocumentDirty = true;
        emit isJsonDocumentDirtyChanged();
    }
}

QList<JsonModelItem *> JsonModel::topLevelItems() const
{
    return d->topLevelItems;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
struct TreeTableModelData
{
    TreeTableModelData () : jsonModel(0) {}
    QStringList columnNames;
    QString recordName;
    JsonModel *jsonModel;

    QList< QVariantList > data;
};

TreeTableModel::TreeTableModel(QObject *parent): QTable(parent)
{
    d = new TreeTableModelData;
    connect(this, SIGNAL(modelReset()), this, SIGNAL(descriptionChanged()));
    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(descriptionChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(descriptionChanged()));
    connect(d->jsonModel, SIGNAL(jsonDocumentModified()), this, SLOT(onJsonDoucumentModified()));
}

TreeTableModel::TreeTableModel(JsonModel *jsonModel)
    : QTable(jsonModel)
{
    d = new TreeTableModelData;
    d->jsonModel = jsonModel;
    connect(this, SIGNAL(modelReset()), this, SIGNAL(descriptionChanged()));
    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(descriptionChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(descriptionChanged()));
    connect(d->jsonModel, SIGNAL(jsonDocumentModified()), this, SLOT(onJsonDoucumentModified()));
}

void TreeTableModel::setTreeModel(const QJsonDocument &jsonDoc)
{
    d->jsonModel->setJsonDocument(jsonDoc);
}

QJsonDocument TreeTableModel::treeModel() const
{
    return d->jsonModel->jsonDocument();
}

QString TreeTableModel::description() const
{
    return QString("Table with %1 rows").arg(d->data.size());
}

void TreeTableModel::setRecordName(const QString &recordName)
{
    if(d->recordName == recordName)
        return;

    d->recordName = recordName;
}

QString TreeTableModel::recordName() const
{
    return d->recordName;
}

//void TreeTableModel::addRows(const QJsonArray &array)
//{
//    QList< QVariantList > data = d->data;
//    QStringList columnNames = d->columnNames;

////    bool firstIteration = true;
//    for(int i=0; i<array.count(); i++)
//    {
//        QJsonObject obj = array.at(i).toObject();
//        if(obj.isEmpty())
//            continue;

//        // merge keys
//        QStringList keys = obj.keys();
//        Q_FOREACH(QString key, keys)
//        {
//            if(columnNames.contains(key))
//                continue;
//            columnNames.append(key);
//        }

//        // load data
//        QVariantList row;
//        row.append(obj);
//        Q_FOREACH(QString key, columnNames)
//        {
//            double value = obj.value(key).toDouble();

//            row.append( value );
//        }

//        data.append(row);
////        firstIteration = false;
//    }

//    // If there has been a change in column-names, we should reset the model
//    bool isReset = (d->columnNames != columnNames);
//    if(isReset)
//        this->beginResetModel();
//    else
//        this->beginInsertRows(QModelIndex(), d->data.size(), data.size()-1);
//    d->columnNames = columnNames;
//    d->data = data;
//    if(isReset)
//    {
//        this->endResetModel();
//        emit columnNamesChanged();
//    }
//    else
//        this->endInsertRows();
//}

//void TreeTableModel::removeRowAt(int row)
//{
//    if(row < 0 || row >= d->data.size())
//        return;

//    this->beginRemoveRows(QModelIndex(), row, row);
//    d->data.removeAt(row);
//    this->endRemoveRows();
//}

//void TreeTableModel::clear()
//{
//    this->beginResetModel();
//    d->columnNames.clear();
//    d->data.clear();
//    this->endResetModel();
//    emit columnNamesChanged();
//}

QJsonArray TreeTableModel::jsonArray() const
{
    QJsonArray ret;

    Q_FOREACH(QVariantList row, d->data)
    {
        if(row.size() <= 1)
            continue;

        row.takeFirst();

        QJsonObject obj;
        for(int i=0; i<d->columnNames.size(); i++)
        {
            const QString colName = d->columnNames.at(i);
            const QVariant value = i < row.size() ? row.at(i) : QVariant();
            obj[colName] = QJsonValue::fromVariant(value);
        }

        ret.append(obj);
    }

    return ret;
}

QStringList TreeTableModel::columnNames() const
{
    return d->columnNames;
}

int TreeTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : d->data.size();
}

int TreeTableModel::columnCount(const QModelIndex &) const
{
    return d->columnNames.size();
}

QVariant TreeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        if(section < 0 || section >= d->columnNames.size())
            return QVariant();
        return d->columnNames.at(section);
    }

    if(section < 0 || section >= d->data.size())
        return QVariant();

    return QVariant(section+1);
}

QVariant TreeTableModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= d->data.size())
        return QVariant();

    QVariantList row = d->data.at(index.row());
    if(role == ObjectRole)
        return row.first();

    row.takeFirst();
    if(role == TableRowRole)
        return row;

    int col = (index.column()+1);
    if(col < 1 || col >= row.size())
        return QVariant();

    return row.at(col);
}

QVariant TreeTableModel::value(int row, const QString &columnName) const
{
    return this->value(row, d->columnNames.indexOf(columnName));
}

bool TreeTableModel::setValue(int row, const QString &columnName, const QVariant &value)
{
    return this->setValue(row, d->columnNames.indexOf(columnName), value);
}

QVariant TreeTableModel::value(int row, int col) const
{
    if(row < 0 || row >= d->data.size())
        return QVariant();

    const QVariantList rowData = d->data.at(row);
    ++col;
    if(col < 1 || col >= rowData.size())
        return QVariant();

    return rowData.at(col);
}

bool TreeTableModel::setValue(int row, int col, const QVariant &value)
{
    if(row < 0 || row >= d->data.size())
        return false;

    QVariantList &rowData = d->data[row];
    ++col;
    if(col < 1 || col >= rowData.size())
        return false;

    rowData[col] = value;

//    QModelIndex zeroIndex = this->index(row, 0);
//    QModelIndex colIndex = this->index(row, col-1);
//    emit dataChanged(zeroIndex, zeroIndex); // for QML
//    emit dataChanged(colIndex, colIndex);

    return true;
}

