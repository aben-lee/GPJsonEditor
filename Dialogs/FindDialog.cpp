

#include <QWidget>
#include "FindDialog.h"
//#include "Table.h"

FindDialog::FindDialog(QWidget *parent)
   : QDialog(parent)
{
   setupUi(this);
   connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateFindButton()));
   connect(findButton, SIGNAL(clicked()), this, SLOT(findButtonClicked()));
   connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void FindDialog::findButtonClicked()
{
   if (findBackwardsCheckBox -> isChecked())
      emit findPreviousSignal();
   else
      emit findNextSignal();
   
}

void FindDialog::updateFindButton()
{
   findButton -> setEnabled(!lineEdit->text().isEmpty());
}

Qt::CaseSensitivity FindDialog::caseSensitivity() const
{
   return caseSensitiveCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
}

int FindDialog::inFormula() const
{
//   return inFormulaCheckBox->isChecked() ? Table::FindInFormula : Table::FindInDisplayText;
    return 0;
}

QString FindDialog::text() const
{
   return lineEdit->text();
}
