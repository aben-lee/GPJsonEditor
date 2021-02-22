


#ifndef NewFeatureDialog_H
#define NewFeatureDialog_H

#include "ui_NewFeatureDialog.h"
#include <QDialog>

class NewFeatureDialog : public QDialog, public Ui::NewFeatureDialog
{
   Q_OBJECT
public:
      explicit NewFeatureDialog(QWidget* = 0);
        ~NewFeatureDialog();

private slots:

      void showGroupBoxSize(bool checked);

};

#endif
