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

#include <ECF/GuiApplication>
#include <QMessageBox>
#include <QDebug>
#include <QWidget>
//#include "FeatureWidget.h"
//#include "CollectWindow.h"
#include "ApplicationWindow.h"
//#include "TableView.h"

int main(int argc, char** argv)
{
    ECF::GuiApplication  a(argc, argv);

//    FeatureWidget view;

//    TableView view;
//    CollectWindow view;
    ApplicationWindow view;
    view.setMinimumSize(800, 600);
    view.show();
    return a.processArgumentsAndExec();
}
