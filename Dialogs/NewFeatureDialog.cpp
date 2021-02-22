
#include "NewFeatureDialog.h"
#include <QButtonGroup>

NewFeatureDialog::NewFeatureDialog(QWidget *parent)
   : QDialog(parent)
{
    setupUi(this);
    setFixedSize(sizeHint());

    QButtonGroup *typeGroup = new QButtonGroup(this);
    typeGroup->addButton(radioButtonSheet,1);
    typeGroup->addButton(radioButtonMatrix,2);
    typeGroup->addButton(radioButtonBinary,3);
    typeGroup->addButton(radioButtonURI,4);
    connect(typeGroup, QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled),
        [=](QAbstractButton *button, bool checked){
        if( radioButtonSheet->isChecked() || radioButtonMatrix->isChecked())
            groupBoxSize->show();
        else
            groupBoxSize->hide();
        Q_UNUSED(button)
        Q_UNUSED(checked)
    });

    connect(radioButtonSheet,SIGNAL(toggled(bool)),this,SLOT(showGroupBoxSize(bool)));
    connect(radioButtonMatrix,SIGNAL(toggled(bool)),this,SLOT(showGroupBoxSize(bool)));
}

NewFeatureDialog::~NewFeatureDialog()
{

}

void NewFeatureDialog::showGroupBoxSize(bool checked)
{
    if(checked)
        groupBoxSize->show();
    else
        groupBoxSize->hide();
}
