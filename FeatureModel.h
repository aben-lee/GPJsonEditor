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

#ifndef FeatureModel_H
#define FeatureModel_H

#include <QAbstractTableModel>
#include <QAbstractTableModel>
#include <QTextStream>
#include <QFile>
#include <QChar>
#include "gpjson.hpp"

class FeatureModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FeatureModel(QObject *parent = nullptr);
    explicit FeatureModel(QSharedPointer<GPJson::Feature> feature, QObject *parent = nullptr);
        ~FeatureModel();

   Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly )
   bool readOnly() const;
   void setReadOnly(const bool state );

    Q_PROPERTY(QString type READ type /*WRITE setType NOTIFY typeChanged*/)
    QString type() const;
    void setType(const QString &t); // 指定Table 类型{Sheet,Binary,URI,Matrix}
    Q_SIGNAL void typeChanged();

    Q_PROPERTY(QString id READ type WRITE setID NOTIFY idChanged)
    QString id() const;
    void setID(const QString &bytes);
    Q_SIGNAL void idChanged();

    Q_PROPERTY(QJsonObject properties READ properties WRITE setProperties NOTIFY propertiesChanged)
    QJsonObject properties() const;
    void setProperties(const QJsonObject &prop);
    Q_SIGNAL void propertiesChanged();

    QSharedPointer<GPJson::Feature> feature();
    void setFeature(GPJson::Feature *fe);
    bool setFeature(const QByteArray &json, QString &errorInfo);// avoid using parse json, but GPjson:: method
    void reset();

//    void importASCII(const QString &file, int import_mode, const QChar &local_column_separator, int local_ignored_lines,
//                     bool local_rename_columns, bool local_strip_spaces, bool local_simplify_spaces, bool local_convert_to_numeric,
//                     QLocale local_numeric_locale);
//    bool exportASCII(const QString& fname, QChar separator = ';', bool haveHeader = true);

//    bool loadCvs(QString file, QChar separator = ';', bool haveHeader = true);
//    void saveCvs();
//    void saveCvs(QString file);

public:
    // QAbstractItemModel interface
    bool hasIndex( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    virtual QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;
    virtual bool moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count, const QModelIndex &destinationParent, int destinationChild) override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;   

private:
    void swapRows(int row1, int row2);
    bool lessThan(int column, int row1, int row2);
    bool parseCvs();

signals:
   void modified();
   void headerDataChanged(Qt::Orientation orientation, int first, int last);

private:
    QSharedPointer<GPJson::Feature> _feature;
    bool _readOnly;

    // the original file line by line
    QStringList _fileLines;
    QString _file;
    QChar _separator;
    QLocale _localNumeric;
    bool _haveHeader;
    bool _toNumeric;
    bool _internalStatus;

};

#endif // FeatureModel_H
