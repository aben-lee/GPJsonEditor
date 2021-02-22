/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2011 SCHUTZ Sacha
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef QJSONMODEL_H
#define QJSONMODEL_H

#include <QAbstractItemModel>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QIcon>

/*   How to use this model
*        QJsonModel
*        QJsonModel is a json tree model class for Qt5/C++11/Python based on QAbstractItemModel.
*        QJsonModel is under MIT License.
*
*        ![QJsonModel](https://raw.githubusercontent.com/dridk/QJsonmodel/master/screen.png)
*
*        ## Usage C++
*
*        Add `qjsonmodel.cpp` and `qjsonmodel.h` into your project.
*
*        ```cpp
*        QJsonModel * model = new QJsonModel;
*        QTreeView * view = new QTreeView;
*        view->setModel(model);
*        model->load("example.json")
*        ```
*
*        ## Usage Python
*
*        Add `qjsonmodel.py` to your `PYTHONPATH`.
*
*        ```bash
*        $ pip install Qt.py
*        ```
*
*        ```python
*        import json
*        import qjsonmodel
*
*        model = QJsonModel()
*        view = QTreeView()
*        view.setModel(model)
*
*        with open("example.json") as f:
*            model.load(json.load(f))
*/

class QJsonModel;
class QJsonItem;

class QJsonTreeItem
{
public:
    QJsonTreeItem(QJsonTreeItem * parent = nullptr);
    ~QJsonTreeItem();
    void appendChild(QJsonTreeItem * item);
    void removeChild(QJsonTreeItem * item);
    void removeAt(int i);
    QJsonTreeItem *child(int row);
    QJsonTreeItem *parent();
    int childCount() const;
    int row() const;
    void setKey(const QString& key);
    void setValue(const QString& value);
    void setType(const QJsonValue::Type& type);
    QString key() const;
    QString value() const;
    QJsonValue::Type type() const;

    static QJsonTreeItem* load(const QJsonValue& value, QJsonTreeItem * parent = 0);

protected:

private:
    QString mKey;
    QString mValue;
    QJsonValue::Type mType;
    QList<QJsonTreeItem*> mChilds;
    QJsonTreeItem * mParent;

};

//---------------------------------------------------

class QJsonModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QJsonModel(QObject *parent = nullptr);
    QJsonModel(const QString& fileName, QObject *parent = nullptr);
    QJsonModel(QIODevice * device, QObject *parent = nullptr);
    QJsonModel(const QByteArray& json, QObject *parent = nullptr);
    ~QJsonModel();

    bool insertJsonPair(const QModelIndex &index, const QString &key, const QJsonValue value= QJsonValue("") );
    bool deleteJsonPair(const QModelIndex &index = QModelIndex());

    bool load(const QString& fileName);
    bool load(QIODevice * device);
    bool loadJson(const QByteArray& json);
    bool loadJson(const QJsonDocument& jdoc);
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QJsonDocument json() const;

private:
    QJsonValue genJson(QJsonTreeItem *) const;

    QJsonTreeItem * mRootItem;
    QStringList mHeaders;
};

#endif // QJSONMODEL_H
