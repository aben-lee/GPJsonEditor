content  =  {
    object : [
        {name : "controller", title : ""   },
        { name : "calculatorWidget",  type : "widget", parent: "MainWindowComponent.rightDockArea",
            property : {
                windowTitle  : qsTr("Calculator"),
            }
        },
        { name : "calculatorMenu", type : "menu",  parent : "MainWindowComponent.menuBar",
            before : "About" },
        { name : "clearHistory", type : "action", parent : "MainWindowComponent.helpMenu",
            property : {
                title : qsTr("Clear History"),
            },
            connection : {
                "triggered()" : "ScientificCalculatorComponent.calculatorWidget::clearHistory()"
            }
        },
        { name : "clearExpression", type : "action", parent : "MainWindowComponent.helpMenu",
            property : {
                title : qsTr("Clear Expression"),
            },
            connection : {
                "triggered()" : "ScientificCalculatorComponent.calculatorWidget::clearHistory()"
            }
        },
    ]
}



