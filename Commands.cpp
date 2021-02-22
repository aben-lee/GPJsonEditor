

#include <iostream> //NOTE: for debugging
#include <QtGui>
#include "Commands.h"

////////////////////////////////
/// \brief InsertColumns::InsertColumns
/// \param t
/// \param pos
/// \param columns
/// \param parent
///

InsertColumnsCommand::InsertColumnsCommand(FeatureWidget *t, int pos, int am, QUndoCommand *parent)
    :  QUndoCommand(parent),_table(t)
{
    setText(QObject::tr("Insert %1 column(s)").arg(am));
    _position = pos == FeatureWidget::InsertAfter ? 1 : 0;
    _position += _table ->currentColumn();
    if (_position == -1 ) _position = 0;
    _columns =  am;
}

void InsertColumnsCommand::undo()
{
    _table->removeColumns(_position, _columns);

}

void InsertColumnsCommand::redo()
{
    _table->insertColumns(_position, _columns);
}
/////////////////
/// \brief InsertRowsCommand::InsertRowsCommand
/// \param t
/// \param pos
/// \param rows
/// \param parent
///

InsertRowsCommand::InsertRowsCommand(FeatureWidget *t, int pos, int am, QUndoCommand *parent)
    :  QUndoCommand(parent),_table(t)
{
       setText(QObject::tr("Insert %1 row(s)").arg(am));
       _position = pos == FeatureWidget::InsertAfter ? 1 : 0;
       _position += _table->currentRow();
        if (_position == -1 ) _position = 0;
       _amount =  am;
}

void InsertRowsCommand::undo()
{
    _table->removeRows(_position, _amount);
}

void InsertRowsCommand::redo()
{
    _table->insertRows(_position, _amount);
}

//////////////////////////////
/// \brief CutCommand::CutCommand
/// \param t
///
CutCommand::CutCommand(FeatureWidget *t)
    : _table(t)
{
    setText(QObject::tr("Cut"));
    _table -> copy(_oldData);
    _select = _table -> getSelected();
}

void CutCommand::undo()
{
      _table -> paste(_oldData, _select.first().top(), _select.first().left() );
}

void CutCommand::redo()
{
    _table -> deleteContents(_select);
    QMimeData *newMimeData = new QMimeData;
    newMimeData -> setData(CELL_TYPE, _oldData.data(CELL_TYPE));
    QApplication::clipboard() -> setMimeData(newMimeData);
}

/////////////////////
/// \brief PasteCommand::PasteCommand
/// \param t
///
PasteCommand::PasteCommand(FeatureWidget *t)
{
    setText(QObject::tr("Paste"));
    _table = t;

    if (QApplication::clipboard() -> mimeData() -> hasFormat(CELL_TYPE))
    {
       QByteArray byteArray = QApplication::clipboard() -> mimeData() -> data(CELL_TYPE);
       QDataStream inStream(&byteArray, QIODevice::ReadOnly);
       _newData.setData(CELL_TYPE, byteArray);
       _row = _table -> currentRow();
       _column = _table -> currentColumn();
       int numOfRows;
       int numOfColumns;
       inStream >> numOfRows >> numOfColumns;

       _oldData.setData(CELL_TYPE, _table -> getItemsInByteArray(_row, _column, _row+numOfRows-1, _column+numOfColumns-1));
    }
    else if (QApplication::clipboard() -> mimeData() -> hasText())
    {
       QString string = QApplication::clipboard() -> mimeData() -> text();
       _newData.setText(string);
       _row = _table -> currentRow();
       _column = _table -> currentColumn();
       int rowCount = string.count('\n')+1;
       int columnCount = string.count('\t')/rowCount + 1;

       _oldData.setData(CELL_TYPE, _table -> getItemsInByteArray(_row, _column, _row+rowCount-1, _column+columnCount-1));
    }
}

void PasteCommand::undo()
{
    if (_newData.hasFormat(CELL_TYPE) || _newData.hasText())
       _table -> paste(_oldData, _row, _column);
}

void PasteCommand::redo()
{
    if (_newData.hasFormat(CELL_TYPE) || _newData.hasText())
       _table -> paste(_newData, _row, _column);
}

/////////////////////////////
/// \brief DeleteEntireRows::DeleteEntireRows
///
DeleteEntireRows::DeleteEntireRows(FeatureWidget *t, int pos, int am)
{
    setText(QObject::tr("Delete Row(s)"));
    _table = t;
    _position = pos;
    _amount = am;

    _oldData.setData(CELL_TYPE, _table -> getItemsInByteArray(_position, 0, _position+_amount-1, _table->columnCount()-1));
}

void DeleteEntireRows::undo()
{
    _table -> insertRows(_position, _amount);
    _table -> paste(_oldData, _position, 0);
}

void DeleteEntireRows::redo()
{
   _table->removeRows(_position,_amount);
}

///////////////////////////////////////
/// \brief DeleteContents::DeleteContents
/// \param t
///
DeleteContents::DeleteContents(FeatureWidget *t)
    : _table (t)
{
    setText(QObject::tr("Delete"));
    _select = _table -> getSelected();
    _table -> copy(_oldData);
}

void DeleteContents::undo()
{
    if (_oldData.hasFormat(CELL_TYPE))
        _table -> paste(_oldData, _select.first().top(), _select.first().left());
}

void DeleteContents::redo()
{
    if (_oldData.hasFormat(CELL_TYPE))
       _table -> deleteContents(_select);
}


DeleteEntireColumns::DeleteEntireColumns(FeatureWidget *t, int pos, int am)
{
    setText(QObject::tr("Delete Column(s)"));
    _table = t;
    _position = pos;
    _amount = am;
    _oldData.setData(CELL_TYPE, _table -> getItemsInByteArray(0, _position, _table->rowCount()-1, _position+_amount-1));
}

void DeleteEntireColumns::undo()
{
    _table -> insertColumns(_position, _amount);
    _table -> paste(_oldData, 0, _position);
}

void DeleteEntireColumns::redo()
{
   _table -> removeColumns(_position, _amount);
}

DataChanged::DataChanged(FeatureWidget *t, int r, int c, const QVariant &od, const QVariant &nd)
    : _table(t),_row(r),_column(c), _oldData(od), _newData(nd)
{
    setText(QObject::tr("Modify Cell"));
}

void DataChanged::undo()
{
    _table -> replace(_row, _column, _oldData);
    _table -> setCurrentCell(_row, _column);
}

void DataChanged::redo()
{
    _table -> replace(_row, _column, _newData);
    _table -> setCurrentCell(_row, _column);
}

///////////////////////////////////////////////////////////
/// \brief ChangeCellsFont::ChangeCellsFont
/// \param t
/// \param f
///
ChangeCellsFont::ChangeCellsFont(FeatureWidget *t, QFont f)
    :  _table(t),_newFont(f)
{
    setText(QObject::tr("Change Font"));
    _oldFonts = _table -> font();
}

void ChangeCellsFont::undo()
{
    _table -> setFont(_oldFonts);
}

void ChangeCellsFont::redo()
{
    _table -> setFont(_newFont);
}

///////////////////////////////////////////////////////////////////////
/// \brief ChangeCellsEditor::ChangeCellsEditor
///
//ChangeCellsEditor::ChangeCellsEditor(FeatureWidget *t , QItemSelectionRange r, int e)
//    : _table(t), _range(r),_newEditor(e)
//{
//    setText(QObject::tr("Change Data Format"));

//    for (int i=0; i<_range.height(); i++)
//       for (int j=0; j<_range.width(); j++)
//           _oldEditors.append(_table -> getItemEditor(i+_range.top(), j+_range.left()));
//}

//void ChangeCellsEditor::undo()
//{

//}

//void ChangeCellsEditor::redo()
//{

//}
