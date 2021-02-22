/****************************************************************************
**
** Copyright (C) My Organization
**
** Use of this file is limited according to the terms specified by
** My Organization.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef SCIENTIFICCALCULATORCOMPONENT_H
#define SCIENTIFICCALCULATORCOMPONENT_H

#include <ECF/GuiComponent>

struct ScientificCalculatorComponentData;
class ScientificCalculatorComponent : public ECF::GuiComponent
{
    Q_OBJECT

public:

    ScientificCalculatorComponent();
        ~ScientificCalculatorComponent();

protected:
    void contentLoadEvent(ECF::ContentLoadEvent *e);
    QObject *loadObject(const QString &name, const QVariantMap &info);
    QWidget *loadWidget(const QString &name, const QVariantMap &info);

    void  initializeEvent (ECF::InitializeEvent *e);

    void settingsLoadEvent(ECF::SettingsLoadEvent *e);
    void settingsUnloadEvent(ECF::SettingsUnloadEvent *e);

public:
    // IContainer implementation
    QObject* containerObject();
    
private:
    ScientificCalculatorComponentData* d;
};

#endif

