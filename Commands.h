
#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QMimeData>
#include <QPoint>
#include <QModelIndex>
#include "FeatureWidget.h"

class DataChanged : public QUndoCommand
{
   public:
      DataChanged(FeatureWidget*, int, int, const QVariant &, const QVariant &);
      virtual void undo();
      virtual void redo();

   private:
      FeatureWidget *_table;
      int _row;
      int _column;
      QVariant _oldData;
      QVariant _newData;
};

class InsertColumnsCommand : public QUndoCommand
{
   public:
      InsertColumnsCommand(FeatureWidget *t, int pos, int am, QUndoCommand * parent = 0 );
      virtual void undo();
      virtual void redo();

   private:
      FeatureWidget *_table;
      int _columns;  //! The new columns
      int _position;
      int _amount;              //! columns count before the command
};

class InsertRowsCommand : public QUndoCommand
{
   public:
      InsertRowsCommand(FeatureWidget *t, int pos, int am, QUndoCommand * parent = 0);
      virtual void undo();
      virtual void redo();

   private:
      FeatureWidget *_table;
      int _amount;  //! The new rows number
      int _position;
};

class CutCommand : public QUndoCommand
{
   public:
      CutCommand(FeatureWidget *t);
      virtual void undo();
      virtual void redo();

   private:
      FeatureWidget  *_table;
      QMimeData _oldData;
      QItemSelection _select;
};

class PasteCommand : public QUndoCommand
{
   public:
      PasteCommand(FeatureWidget*);
      virtual void undo();
      virtual void redo();

   private:
      FeatureWidget *_table;
      QMimeData _oldData;
      QMimeData _newData;
      int _row;
      int _column;
};

class DeleteContents : public QUndoCommand
{
   public:
      DeleteContents(FeatureWidget* t);
      virtual void undo();
      virtual void redo();
      
   private:
      FeatureWidget *_table;
      QItemSelection _select;
      QMimeData _oldData;
};

class DeleteEntireRows : public QUndoCommand
{
   public:
      DeleteEntireRows(FeatureWidget* t, int, int);
      virtual void undo();
      virtual void redo();
      
   private:
      FeatureWidget *_table;
      int _position;
      int _amount;
      QMimeData _oldData;
};

class DeleteEntireColumns : public QUndoCommand
{
   public:
      DeleteEntireColumns(FeatureWidget*t, int pos, int am);
      virtual void undo();
      virtual void redo();
      
   private:
      FeatureWidget *_table;
      int _position;
      int _amount;
      QMimeData _oldData;
};

class ChangeCellsFont : public QUndoCommand
{
   public:
      ChangeCellsFont(FeatureWidget* t, QFont);
      virtual void undo();
      virtual void redo();

   private:
      FeatureWidget *_table;
      QFont _oldFonts;
      QFont _newFont;
};

//class ChangeCellsEditor : public QUndoCommand
//{
//   public:
//      ChangeCellsEditor(FeatureWidget*, QItemSelectionRange, int);
//      virtual void undo();
//      virtual void redo();
      
//   private:
//      FeatureWidget *_table;
//      QItemSelectionRange _range;
//      QList<int> _oldEditors;
//      int _newEditor;
//};

//class ChangeCellsAlignment : public QUndoCommand
//{
//   public:
//      ChangeCellsAlignment(FeatureWidget*, QTableWidgetSelectionRange, int);
//      virtual void undo();
//      virtual void redo();
      
//   private:
//      FeatureWidget *table;
//      QTableWidgetSelectionRange range;
//      QList<int> oldAlignments;
//      int newAlignment;
//};

//class ChangeCellsBackground : public QUndoCommand
//{
//   public:
//      ChangeCellsBackground(FeatureWidget*, QTableWidgetSelectionRange, QColor);
//      virtual void undo();
//      virtual void redo();
      
//   private:
//      FeatureWidget *table;
//      QTableWidgetSelectionRange range;
//      QList<QColor> oldBackgrounds;
//      QColor newBackground;
//};

//class ChangeCellsForeground : public QUndoCommand
//{
//   public:
//      ChangeCellsForeground(FeatureWidget*, QTableWidgetSelectionRange, QColor);
//      virtual void undo();
//      virtual void redo();
      
//   private:
//      FeatureWidget *table;
//      QTableWidgetSelectionRange range;
//      QList<QColor> oldForegrounds;
//      QColor newForeground;
//};

//class SortCommand : public QUndoCommand
//{
//   public:
//      SortCommand(FeatureWidget *t, const TableSort &s);
//      virtual void undo();
//      virtual void redo();
      
//   private:
//      FeatureWidget *table;
//      QMimeData oldData;
//      QItemSelectionRange range;
//      TableSort tableSort;
//};

#endif
