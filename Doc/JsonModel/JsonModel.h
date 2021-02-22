#ifndef JSONMODEL_H
#define JSONMODEL_H

#include <QJSValue>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QQmlListProperty>
#include <QAbstractItemModel>

#include "IJsonModel.h"

class JsonTableModel;
class JsonListModel;
class JsonModelItem;
class LocationModel;
class CoilLocationModel;

struct JsonModelData;
class JsonModel : public IJsonModel
{
    Q_OBJECT
    Q_INTERFACES(IJsonModel)

public:
    JsonModel(QObject *parent = 0);
    ~JsonModel();

    Q_INVOKABLE void setJsonDocumentFromByteArray(const QByteArray &bytes) {
        QJsonDocument doc = QJsonDocument::fromJson(bytes);
        this->setJsonDocument(doc);
    }

    void setJsonDocumentFromFile(const QString &fileName);
    void clear();

    Q_PROPERTY(QJsonDocument jsonDocument READ jsonDocument WRITE setJsonDocument NOTIFY jsonDocumentModified)
    void setJsonDocument(const QJsonDocument &jsonDoc);
    QJsonDocument jsonDocument() const;
    Q_SIGNAL void jsonDocumentModified();

    Q_PROPERTY(bool isJsonDocumentDirty READ isJsonDocumentDirty NOTIFY isJsonDocumentDirtyChanged)
    bool isJsonDocumentDirty() const;
    Q_SIGNAL void isJsonDocumentDirtyChanged();

    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    void setFilter(const QString &filter);
    QString filter() const;
    Q_SIGNAL void filterChanged();

    Q_INVOKABLE void reconstructJsonDocument();

    Q_PROPERTY(QObject* listModel READ listModelObject CONSTANT)
    JsonListModel *listModel() const;

    Q_INVOKABLE QObject* tableModel(int index = 0) const;

    Q_INVOKABLE int tableModelListCount() const;
    QObject *listModelObject();

    QString xyvSurferData() const;
    QString xyzvSurferData() const;
    QString surferGridData() const;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QHash<int,QByteArray> roleNames() const;

    // access to topLevelItems
    void addToTopLevelItems(JsonModelItem* item);
    void removeFromTopLevelItems(JsonModelItem* item);
    int topLevelItemCount();
    JsonModelItem* topLevelItemAt(int index);
    int topLevelIndexOf(JsonModelItem* item);

    QList<JsonModelItem*>& getFlatList();
    int flatListIndexOf(JsonModelItem* item);

    // IJsonModel implementation
    QList<IJsonModelItem*> fetchHeaderItemsInRecords(const QString &fieldName) const;
    QModelIndex index(IJsonModelItem *item, int column=0) const;

protected:
    void itemDataChanged(JsonModelItem *item);
    virtual void load(const QJsonValue &value, JsonModelItem *parent=0);
    void itemAdded(JsonModelItem *item);
    void itemRemoved(JsonModelItem *item);

    virtual QVariant toVariant(const QString &parentKey, const QString &key, const QJsonValue &value);
    virtual QJsonValue fromVariant(const QString &key, const QVariant &value) const;
    virtual bool hasChildItems(const QString &key, const QJsonValue &value) const;

    Q_SLOT void setJsonDocumentDirty();

    QList<JsonModelItem*> topLevelItems() const;

private:
    friend class JsonListModel;
    friend class JsonModelItem;
    friend class JsonTableModel;
    friend class LocationModel;
    friend class CoilLocationModel;
    JsonModelData *d;
};

struct JsonTableModelData;
class JsonTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    JsonTableModel(JsonModel *jsonModel);
    ~JsonTableModel() { }

    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    QString description() const;
    Q_SIGNAL void descriptionChanged();

    // Additionally added for showing data in graph
    Q_PROPERTY(QString recordName READ recordName WRITE setRecordName NOTIFY recordNameChanged)
    void setRecordName(const QString &recordName);
    QString recordName() const;
    Q_SIGNAL void recordNameChanged();

    Q_INVOKABLE double minValue(int column) const;
    Q_INVOKABLE double minValue(const QString &columnName) const;
    Q_INVOKABLE double maxValue(int column) const;
    Q_INVOKABLE double maxValue(const QString &columnName) const;

    // End

    void addRow(const QJsonObject &object) {
        QJsonArray array;
        array.append(object);
        this->addRows(array);
    }
    void addRows(const QJsonArray &array);
    Q_INVOKABLE void removeRowAt(int row);
    void clear();

    QJsonArray jsonArray() const;

    Q_PROPERTY(QStringList columnNames READ columnNames NOTIFY columnNamesChanged)
    QStringList columnNames() const;
    Q_SIGNAL void columnNamesChanged();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &/*parent*/) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    enum Roles {
        ObjectRole = Qt::UserRole+1,
        TableRowRole
    };
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int,QByteArray> roleNames() const {
        QHash<int,QByteArray> roles;
        roles[ObjectRole] = "object";
        roles[TableRowRole] = "tableRow";
        return roles;
    }
    Q_INVOKABLE QVariant value(int row, const QString &columnName) const;
    Q_INVOKABLE bool setValue(int row, const QString &columnName, const QVariant &value);
    Q_INVOKABLE QVariant value(int row, int col) const;
    Q_INVOKABLE bool setValue(int row, int col, const QVariant &value);

protected slots:
    void onJsonDoucumentModified();

private:
    JsonTableModelData *d;
};

#endif // JSONMODEL_H
