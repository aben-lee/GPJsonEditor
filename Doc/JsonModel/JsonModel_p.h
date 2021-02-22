#ifndef JSONMODEL_P_H
#define JSONMODEL_P_H

#include "JsonModel.h"
#include "IJsonModelItem.h"
#include <QAbstractListModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QtDebug>

struct JsonModelItemData;
class JsonModelItem : public QObject, virtual public IJsonModelItem
{
    Q_OBJECT
    Q_INTERFACES(IJsonModelItem)

public:
    JsonModelItem(const QString &key, const QVariant &value, JsonModel *model);
    JsonModelItem(const QString &key, const QVariant &value, JsonModelItem *parent);
    ~JsonModelItem();

    QJsonValue jsonValue() const;

    // IJsonModelItem functions
    IJsonModel *model() const;
    IJsonModelItem *parentItem() const;
    JsonModelItem *parent() const;
    int row() const;

    int column() const { return 0; }
    int flatRow() const;
    void applyFilter(const QString &filter);

    int depth() const;
    int childCount() const;
    JsonModelItem *childAt(int index) const;
    QString key() const;

    QString fieldName(bool includeRoot=false) const;
    void setValue(const QVariant &data);
    QVariant value() const;
    int valueType() const;
    QString valueTypeName() const;
    bool canExpand() const;
    bool setExpanded(bool val);
    bool isExpanded() const;
    bool isVisible() const;
    JsonModelItem *itemForFieldName(const QString &fieldName) const;

    // access to children list
    void addTochildList(JsonModelItem* item);
    void removeFromChildList(JsonModelItem* item);
    int indexOfChildItem(JsonModelItem* item) const;

private:
    friend class JsonModel;
    JsonModelItemData *d;
};

class JsonListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    JsonListModel(JsonModel *parent);
    ~JsonListModel() { }

    Q_PROPERTY(QJsonDocument jsonDocument READ jsonDocument WRITE setJsonDocument NOTIFY jsonDocumentModified)
    void setJsonDocument(const QJsonDocument &jsonDoc) { m_model->setJsonDocument(jsonDoc); }
    QJsonDocument jsonDocument() const { return m_model->jsonDocument(); }
    Q_SIGNAL void jsonDocumentModified();

    Q_PROPERTY(bool isJsonDocumentDirty READ isJsonDocumentDirty NOTIFY isJsonDocumentDirtyChanged)
    bool isJsonDocumentDirty() const { return m_model->isJsonDocumentDirty(); }
    Q_SIGNAL void isJsonDocumentDirtyChanged();

    void clear();

    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    void setFilter(const QString &filter) { m_model->setFilter(filter); }
    QString filter() const { return m_model->filter(); }
    Q_SIGNAL void filterChanged();

    Q_INVOKABLE void reconstructJsonDocument() { m_model->reconstructJsonDocument(); }

    Q_INVOKABLE bool changeValue(int row, const QVariant &data) {
        if(row < 0 || row >= m_flatList.size()) return false;
        m_flatList.at(row)->setValue(data);
        return true;
    }
    Q_INVOKABLE QString key(int row) const {
        if(row < 0 || row >= m_flatList.size()) return QString();
        return m_flatList.at(row)->key();
    }
    Q_INVOKABLE int valueType(int row) const {
        if(row < 0 || row >= m_flatList.size()) return -1;
        return m_flatList.at(row)->valueType();
    }
    Q_INVOKABLE QString valueTypeName(int row) const {
        if(row < 0 || row >= m_flatList.size()) return QString();
        return m_flatList.at(row)->valueTypeName();
    }

    Q_INVOKABLE void setExpanded(int row, bool val);

    Q_INVOKABLE bool isExpanded(int row) {
        if(row < 0 || row >= m_flatList.size()) return false;
        return m_flatList.at(row)->isExpanded();
    }

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent) const {
        return parent.isValid() ? 0 : m_flatList.size();
    }
    int columnCount(const QModelIndex &/*parent*/=QModelIndex()) const {
        return m_model->columnCount(QModelIndex())+5;
    }
    QModelIndex index(int row, int column=0, const QModelIndex &parent=QModelIndex()) const {
        if(parent.isValid()) return QModelIndex();
        if(row < 0 || row >= m_flatList.size()) return QModelIndex();
        return this->createIndex(row, column, static_cast<IJsonModelItem*>(m_flatList.at(row)));
    }
    QModelIndex parent(const QModelIndex &/*child*/) const { return QModelIndex(); }

    enum Roles {
        DepthRole = JsonModel::MaxRole+1,
        PairRole,
        CanExpandRole,
        IsExpandedRole,
        IsVisibleRole,
        IsFirstChildRole
    };
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int,QByteArray> roleNames() const;
    bool setData(const QModelIndex &index, const QVariant &value, int role) {
        return m_model->setData(this->mapToSource(index), value, role);
    }
    Qt::ItemFlags flags(const QModelIndex &index) const {
        return m_model->flags(this->mapToSource(index));
    }

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex &index) const;

protected:
    Q_SLOT void onSourceDataChanged(const QModelIndex &sourceStart, const QModelIndex &sourceEnd) {
        const QModelIndex start = this->mapFromSource(sourceStart);
        const QModelIndex end = this->mapFromSource(sourceEnd);
        emit dataChanged(start, end);
    }

private:
    friend class JsonModel;
    JsonModel *m_model;
    QList<JsonModelItem*> &m_flatList;
};

#endif // JSONMODEL_P_H
