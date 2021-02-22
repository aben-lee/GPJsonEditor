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

#include "FeatureModel.h"
#include <QString>
#include <QApplication>
#include <QMessageBox>

//FeatureModel::FeatureModel(QObject *parent)
//    : QAbstractTableModel(parent), _file(""), _separator(';'),  _localNumeric(QLocale::C),
//      _haveHeader(true), _toNumeric(true),_internalStatus(false)
//{

//}

FeatureModel::FeatureModel(QObject *parent)
    : QAbstractTableModel(parent), _feature(new GPJson::Feature)
{
    _readOnly = false;
}

FeatureModel::FeatureModel(QSharedPointer<GPJson::Feature> feature, QObject *parent)
    : QAbstractTableModel(parent), _feature(feature)
{
    _readOnly = false;
}

FeatureModel::~FeatureModel()
{
    _feature.clear();
}

void FeatureModel::setReadOnly(const bool state)
{
    _readOnly = state;
}

bool FeatureModel::readOnly()const
{
    return  _readOnly;
}

QString FeatureModel::type() const
{
    return  _feature->dataset.type;
}

void FeatureModel::setType(const QString &t)
{
    if(_feature->dataset.type == t)
        return;

    if(t.toLower() == "sheet")
        _feature->dataset.type = "Sheet";
    else if(t.toLower() == "matirx")
        _feature->dataset.type = "Matrix";
    else if(t.toLower() == "binary")
        _feature->dataset.type = "Binary";
    else if(t.toLower() == "uri")
        _feature->dataset.type = "URI";
    else
        return;

    emit typeChanged();
}

QString FeatureModel::id() const
{
    return  QString(_feature->id);
}

void FeatureModel::setID(const QString &bytes)
{
    if(_feature->id == bytes.toUtf8())
        return;

    _feature->id = bytes.toUtf8();
    emit idChanged();
}

QJsonObject FeatureModel::properties() const
{
    return _feature->properties;
}

void FeatureModel::setProperties(const QJsonObject &prop)
{
    if(_feature->properties == prop)
        return;

    _feature->properties = prop;
    emit propertiesChanged();
}

QSharedPointer<GPJson::Feature> FeatureModel::feature()
{
    return _feature;
}

void FeatureModel::setFeature(GPJson::Feature *fe)
{
    if(_feature.data() == fe)
        return;

     emit beginResetModel();
     _feature.clear();
     _feature.reset(fe);
     emit headerDataChanged(Qt::Horizontal,0,_feature->dataset.dimensions.size()-1);
     emit modified();
     emit endResetModel();
}

bool FeatureModel::setFeature(const QByteArray &json, QString &errorInfo)
{
    emit beginResetModel();
    QJsonParseError *info = new QJsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(json,info);
    if(info->error == QJsonParseError::NoError){
        if(_feature->fromDocument(QJsonDocument::fromJson(json),errorInfo))
            return true;
        else
            return false;
    }
    else
    {
        errorInfo = info->errorString();
        return false;
    }
    emit headerDataChanged(Qt::Horizontal,0,_feature->dataset.dimensions.size()-1);
    emit modified();
    emit endResetModel();
}

void FeatureModel::reset()
{
    emit beginResetModel();
    _feature.reset(new GPJson::Feature());
    emit headerDataChanged(Qt::Horizontal,0,_feature->dataset.dimensions.size()-1);
    emit modified();
    emit endResetModel();
}

//void FeatureModel::importASCII(const QString &file, int import_mode, const QChar &local_column_separator,
//                               int local_ignored_lines, bool local_rename_columns, bool local_strip_spaces,
//                               bool local_simplify_spaces, bool local_convert_to_numeric, QLocale local_numeric_locale)
//{
//    if (file.isEmpty())
//        return;

//    _file = file;
//    _separator = local_column_separator;
//    _haveHeader = local_rename_columns;
//    _toNumeric = local_convert_to_numeric;
//    _localNumeric = local_numeric_locale;

//    // this is very much a special case, and thus is handled completely in its own block
//    if (import_mode == ImportASCIIDialog::NewTables ) {
//        reset();
//        QFile data(_file);
//        if (data.open(QFile::ReadOnly | QFile::Text))
//        {
//            QTextStream in(&data);
//            in.setCodec("UTF-8");
//            QString line;
//            if (local_strip_spaces) in.skipWhiteSpace();
//            for(int i = 0; i < local_ignored_lines; ++i) in.readLineInto(&line);
//            while (in.readLineInto(&line))
//            {
//                if(local_simplify_spaces) line = line.simplified();
//                _fileLines.append(line);
//            }
//            data.close();
//        }
//        _internalStatus = parseCvs();
//        return;
//    }
//    else if(import_mode == ImportASCIIDialog::NewRows)
//    {
//        QFile data(_file);
//        if (data.open(QFile::ReadOnly | QFile::Text))
//        {
//            QTextStream in(&data);
//            in.setCodec("UTF-8");
//            QString line;
//            if (local_strip_spaces) in.skipWhiteSpace();
//            for(int i = 0; i < local_ignored_lines; ++i) in.readLineInto(&line);
//            while (in.readLineInto(&line))
//            {
//                insertColumns(columnCount()-1,1);
//                if(local_simplify_spaces) line = line.simplified();
//                QStringList strList = line.split(_separator);
//                for(int i = 0; i < strList.count(); ++i){
//                      QVariant token;
//                      _toNumeric ? token = strList[i].toDouble() : token = strList[i];
//                       setData(this->index(i,columnCount()-1), token );
//                }
//            }
//            data.close();
//        }
//        return;
//    }
//    else if(import_mode == ImportASCIIDialog::NewColumns)
//    {
//        QFile data(_file);
//        if (data.open(QFile::ReadOnly | QFile::Text))
//        {
//            QTextStream in(&data);
//            in.setCodec("UTF-8");
//            QString line;
//            if (local_strip_spaces) in.skipWhiteSpace();
//            for(int i = 0; i < local_ignored_lines; ++i) in.readLineInto(&line);
//            while (in.readLineInto(&line))
//            {
//                insertRows(rowCount()-1,1);
//                if(local_simplify_spaces) line = line.simplified();
//                QStringList strList = line.split(_separator);
//                if(local_rename_columns){
//                    setHeaderData(columnCount(), Qt::Horizontal, strList.at(0) );     // first str regard as columns name
//                    strList.removeAt(0);
//                }
//                for(int i = 0; i < strList.count(); ++i){
//                      QVariant token;
//                      _toNumeric ? token = strList[i].toDouble() : token = strList[i];
//                       setData(this->index(rowCount()-1,i), token );
//                }
//            }
//            data.close();
//        }
//        return;
//    }
//    else { // import_mode == ImportASCIIDialog::Overwrite
//        QFile data(_file);
//        if (data.open(QFile::ReadOnly | QFile::Text))
//        {
//            QTextStream in(&data);
//            in.setCodec("UTF-8");
//            QString line;
//            if (local_strip_spaces) in.skipWhiteSpace();
//            for(int i = 0; i < local_ignored_lines; ++i) in.readLineInto(&line);
//            while (in.readLineInto(&line))
//            {
//                if(local_simplify_spaces) line = line.simplified();
//                _fileLines.append(line);
//            }
//            data.close();
//        }
//        _internalStatus = parseCvs();
//        return;
//    }
//}

//bool FeatureModel::exportASCII(const QString &fname, QChar separator, bool haveHeader)
//{
//    QFile file(fname);
//    if ( !file.open( QIODevice::WriteOnly ) )
//      {
//        QApplication::restoreOverrideCursor();
//        QMessageBox::critical(0, tr("ASCII Export Error"),
//                              tr("Could not write to file: <br><h4>")+fname+tr("</h4><p>Please verify that you have the right to write to this location!"));
//        return false;
//      }


//    QTextStream _out(&file);
//    _out.setCodec("UTF-8");
//    _out << "properties = " << QJsonDocument(_feature.properties).toJson(QJsonDocument::Indented) <<Qt::endl;
//    _out << "id : " << _feature.id << Qt::endl;

//    _out << "type : " << _feature.dataset.type << Qt::endl;
//    _out << "-----------------------------------------------------" << Qt::endl;
//    if (haveHeader) {
//        for (auto val : _feature.dataset.dimensions)
//            _out << val.toString().append(separator);
//    }
//    _out << Qt::endl;
//    for (auto row : _feature.dataset.source)
//    {
//        for ( auto val :  row)
//            _out << val.toString().append(separator);
//        _out << Qt::endl;
//    }
//    _out.flush();
//    file.close();
//    return  true;
//}

//bool FeatureModel::loadCvs(QString file,QChar separator, bool haveHeader)
//{
//    _file = file;
//    _separator = separator;
//    _haveHeader = haveHeader;

//    QFile data(_file);
//    if (data.open(QFile::ReadOnly | QFile::Text))
//    {
//        QTextStream in(&data);
//        in.setCodec("UTF-8");
//        QString line;
//        while (in.readLineInto(&line))
//        {
//            _fileLines.append(line);
//        }
//        data.close();
//    }

//    _internalStatus = parseCvs();
//    return _internalStatus;
//}


//void FeatureModel::saveCvs()
//{
//    FeatureModel::saveCvs(_file);
//}

//void FeatureModel::saveCvs(QString file)
//{
//    QFile data(file);
//    if (data.open(QFile::ReadWrite | QFile::Text))
//    {
//        QTextStream _out(&data);
//        _out.setCodec("UTF-8");
//        QString line;
//        foreach (QJsonArray row, _feature.dataset.source)
//        {
//            foreach(QVariant str,  row)
//                _out << str.toString().append(_separator);
//            _out << Qt::endl;
//        }
//        _out.flush();
//        data.close();
//    }
//}

bool FeatureModel::hasIndex(int row, int column, const QModelIndex &parent) const
{
    if( parent.isValid() ) return false;
    if( ( row < 0 ) || ( column < 0 ) ) return false;
    if( column >= columnCount() ) return false;
    if( row >= rowCount() ) return false;
    return true;
}

QModelIndex FeatureModel::index(int row, int column, const QModelIndex &parent) const
{
    if( parent.isValid() )
        return QModelIndex();

    return createIndex(row,column);
}

bool FeatureModel::parseCvs()
{
    bool result = true;
    emit beginResetModel();
    // treat strings declared between " " with separator inside
    QString str("(?:\\%1\\s*|\n|^)(\"(?:(?:\"\")*[^\"]*)*\"|[^\"\\%1\n]*|(?:\n|$))");
    QRegExp exp(str.arg(_separator));
    bool isheader = _haveHeader;
    for(auto line : _fileLines)
    {
        // sanity check for selected separator
        if(line.contains(_separator))
        {
            int pos = 0;
            QJsonArray tokens;
            while ((pos = exp.indexIn(line, pos)) != -1)
            {
               // tokens << exp.cap(1);
               _toNumeric ? tokens << exp.cap(1).toDouble() : tokens << exp.cap(1);
               pos += exp.matchedLength();
            }
            if(isheader)
            {
                _feature->dataset.dimensions = tokens;
                isheader = false;
                emit headerDataChanged(Qt::Horizontal,0,tokens.size()-1);
            }
            else
            {
                _feature->dataset.source.append(tokens);
                emit modified();
            }
        }
        else
        {
            _feature->dataset.source.append(QJsonArray()<<line);
            emit modified();
            result = false;
        }
    }
    emit endResetModel();
    return (_feature->dataset.source.count() > 0 && result);
}

int FeatureModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _feature->dataset.source.count();
}

int FeatureModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if(_feature->dataset.source.count() > 0)
    {
        return _feature->dataset.source.at(0).count();
    }
    else {
        return 0;
    }
}

QVariant FeatureModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    else if (index.row() >= _feature->dataset.source.count() || index.row() < 0)
    {
        return QVariant();
    }
    else if (role == Qt::DisplayRole)
    {
        if(index.row() <= _feature->dataset.source.count() - 1)
        {
            if(index.column() <= _feature->dataset.source.at(index.row()).count() - 1)
            {
                return _feature->dataset.source.at(index.row()).at(index.column());
            }
            else
            {
                return QVariant();
            }
        }
        else
        {
            return QVariant();
        }
    }
    return QVariant();
}

bool FeatureModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int column = index.column();

    if (!index.isValid())
    {
        return false;
    }
    else if(row < 0 || row >= _feature->dataset.source.count()
            || column >= _feature->dataset.source.at(row).count() || column < 0)
    {
        return false;
    }
    else if (role == Qt::EditRole)
    {
        _feature->dataset.source[row][column] = QJsonValue::fromVariant(value);
        emit modified();
        return true;    
    }
    return false;
}

Qt::ItemFlags FeatureModel::flags(const QModelIndex &index) const
{
    if( !index.isValid() ) return Qt::NoItemFlags;
    if( !hasIndex( index.row(), index.column() ) ) return Qt::NoItemFlags;
    Qt::ItemFlags result = Qt::ItemIsSelectable | Qt::ItemIsEnabled ;
    if( !_readOnly ) result |= Qt::ItemIsEditable;
    return result;
}

QVariant FeatureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    else if (orientation == Qt::Horizontal)
    {
        if( section < _feature->dataset.dimensions.count())
        {
            return _feature->dataset.dimensions.at(section);
        }
        else
        {
            return QVariant();
        }
    }
    else
    {
        return section + 1;
    }
}

bool FeatureModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::DisplayRole)
    {
        return false;
    }
    else if (orientation == Qt::Horizontal)
    {
        if(_feature->dataset.dimensions.count() > 0 && section < _feature->dataset.dimensions.count())
        {
            _feature->dataset.dimensions[section] = value.toString();
            emit headerDataChanged(Qt::Horizontal,section,section);
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

void FeatureModel::sort(int column, Qt::SortOrder order)
{
    emit layoutAboutToBeChanged();
    for ( int i = 1; i < rowCount(); )
    {
        if ( order == Qt::AscendingOrder ? lessThan(column, i - 1, i) : lessThan(column, i, i - 1))
        {
            ++i;
        }
        else
        {
            swapRows(i - 1, i);
            --i;
            if ( i == 0 )
            {
                i = 1;
            }
        }
    }
    emit layoutChanged();
}


void FeatureModel::swapRows(int row1, int row2)
{
    qSwap(_feature->dataset.source[row1], _feature->dataset.source[row2]);
}

bool FeatureModel::lessThan(int column, int row1, int row2)
{
    return QString::localeAwareCompare(_feature->dataset.source[row1].at(column).toString(),(_feature->dataset.source[row2].at(column).toString())) < 0;
}

bool FeatureModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(row < 0 && row > _feature->dataset.source.count())
    {
        return false;
    }
    emit beginInsertRows(parent, row, row + count-1);
    if(row >= 0 && row <= _feature->dataset.source.count())
    {
        _feature->dataset.source.insert(row, count, QJsonArray());
    }
    emit endInsertRows();    
    return true;
}

bool FeatureModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    if(column < 0 && column > columnCount(parent))
    {
        return false;
    }
    emit beginInsertColumns(parent, column, column + count-1);
    if(column >= 0 && column <= columnCount(parent))
    {
        for(int j = 0; j < count; j++)
        {
            for (int i = 0; i < _feature->dataset.source.count(); i++)
            {
                _feature->dataset.source[i].insert(column+j, QJsonValue());
            }
           _feature->dataset.dimensions.insert(column+j, QJsonValue());
        }
    }
    emit endInsertColumns();
    return true;
}

bool FeatureModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row < 0 && row > _feature->dataset.source.count())
    {
        return false;
    }
    emit beginRemoveRows(parent, row, row + count-1);
    if(row >= 0 && row < _feature->dataset.source.count())
    {
        count = qMin(count, _feature->dataset.source.count()-row);
        for (int i = 0; i < count; i++)
            _feature->dataset.source.removeAt(row);
    }
    emit endRemoveRows();
    return true;
}

bool FeatureModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    if(column < 0 && column > columnCount(parent))
    {
        return false;
    }
    emit beginRemoveColumns(parent, column, column + count-1);
    if(column >= 0 && column < columnCount(parent) )
    {
        count = qMin(count, _feature->dataset.source.at(0).count()-column);
        for(int j = count-1; j >=0; j--)
        {
            for (int i = 0; i < _feature->dataset.source.count() ; i++)
            {
                _feature->dataset.source[i].removeAt(column+j);
            }
            _feature->dataset.dimensions.removeAt(column+j);
        }
    }
    emit endRemoveColumns();
    return false;
}

bool FeatureModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                            const QModelIndex &destinationParent, int destinationChild)
{
    if(sourceRow < 0 && sourceRow >= _feature->dataset.source.count())
    {
        return false;
    }
    emit beginMoveRows(sourceParent, sourceRow, sourceRow + count, destinationParent, destinationChild);
    if(sourceRow >= 0 && sourceRow < _feature->dataset.source.count())
    {
        count = qMin(count, _feature->dataset.source.count()-sourceRow);
        for (int i = 0; i < count ; i++)
        {
            _feature->dataset.source.move(sourceRow, destinationChild + i);
        }
    }
    emit endMoveRows();
    return true;
}

bool FeatureModel::moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count,
                               const QModelIndex &destinationParent, int destinationChild)
{
    if(sourceColumn < 0 && sourceColumn >= columnCount(sourceParent))
    {
        return false;
    }
    emit beginMoveColumns(sourceParent, sourceColumn, sourceColumn + count, destinationParent, destinationChild);
    if(sourceColumn >= 0 && sourceColumn < columnCount(sourceParent) )
    {
        count = qMin(count, _feature->dataset.source.at(0).count()-sourceColumn);

        for(int j = 0; j < count; j++)
        {
            _feature->dataset.dimensions.insert(destinationChild + j, _feature->dataset.dimensions.takeAt(sourceColumn));
            for (int i = 0; i < _feature->dataset.source.count() ; i++)
            {
                _feature->dataset.source[i].insert(destinationChild + j, _feature->dataset.source[i].takeAt(sourceColumn));
            }
        }
    }
    emit endMoveColumns();
    return false;
}
