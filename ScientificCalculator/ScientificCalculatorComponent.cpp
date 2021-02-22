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

#include "ScientificCalculatorComponent.h"


#include <IScriptHighlighter>
#include <QSyntaxHighlighter>
#include <QSettings>

#include "CalculatorWidget.h"


struct ScientificCalculatorComponentData
{
    ScientificCalculatorComponentData() : calculatorWidget(0) { }

    CalculatorWidget* calculatorWidget;
};


ScientificCalculatorComponent::ScientificCalculatorComponent()
{
    d = new ScientificCalculatorComponentData;
}

ScientificCalculatorComponent::~ScientificCalculatorComponent()
{
    delete d;
}

void ScientificCalculatorComponent::contentLoadEvent(ECF::ContentLoadEvent *e)
{
    if(e->isPreContentLoad())
        e->setContentFile(":/content/ScientificCalculatorComponent.js");

    return ECF::GuiComponent::contentLoadEvent(e);
}

QObject *ScientificCalculatorComponent::loadObject(const QString &name, const QVariantMap &info)
{
    ScientificCalculatorComponent* that = const_cast<ScientificCalculatorComponent*>(this);

    if(name == "controller")
        return that;

    return ECF::GuiComponent::loadObject(name,info);
}

QWidget *ScientificCalculatorComponent::loadWidget(const QString &name, const QVariantMap &info)
{
    if(name == "calculatorWidget")
    {
        if(!d->calculatorWidget)
            d->calculatorWidget = new CalculatorWidget;

        return d->calculatorWidget;
    }
    return ECF::GuiComponent::loadWidget(name,info);
}

void ScientificCalculatorComponent::initializeEvent(ECF::InitializeEvent *e)
{
    // Do nothing.
    return ECF::GuiComponent::initializeEvent(e);
}

void ScientificCalculatorComponent::settingsLoadEvent(ECF::SettingsLoadEvent *e)
{
   if(e->isPreSettingsLoad())
   {
       // do nothing.
   }
   else
   {
       const QSettings *settings = this->settings();
       if(d->calculatorWidget && settings->contains("history") )
       {
           QString history = settings->value("history").toString();
           d->calculatorWidget->setHistory(history);

           IScriptHighlighter* scriptHighlighter
               = gFindObject<IScriptHighlighter>();
           if(scriptHighlighter)
           {
               QSyntaxHighlighter* h = scriptHighlighter->newDefaultHighlighter();
               d->calculatorWidget->setHighlighter(h);
           }
       }
   }
   return ECF::GuiComponent::settingsLoadEvent(e);
}

void ScientificCalculatorComponent::settingsUnloadEvent(ECF::SettingsUnloadEvent *e)
{
    if(e->isPreSettingsUnload())
    {
        QSettings *settings = const_cast<QSettings*>(this->settings());
        // add/remove/alter settings here
        if(d->calculatorWidget)
        {
            QString history = d->calculatorWidget->history();
            settings->setValue("history", QVariant(history) );
        }
    }

    return  ECF::GuiComponent::settingsUnloadEvent(e);
}

QObject* ScientificCalculatorComponent::containerObject()
{
    return this;
}

ECF_EXPORT_COMPONENT(ScientificCalculatorComponent)

