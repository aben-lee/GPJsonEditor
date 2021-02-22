INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/../GPjson $$PWD/Dialogs

# Input
HEADERS += \
    $$PWD/../GPjson/gpjson.hpp \
    $$PWD/ApplicationWindow.h \
    $$PWD/CollectWidget.h \
#    $$PWD/CollectWindow.h \
    $$PWD/Commands.h \
    $$PWD/Dialogs/FindDialog.h \
    $$PWD/Dialogs/GraphicPropertiesDialog.h \
    $$PWD/Dialogs/InsertRCDialog.h \
    $$PWD/Dialogs/MySpinBox.h \
    $$PWD/Dialogs/NewFeatureDialog.h \
#    $$PWD/Dialogs/SortDialog.h \
    $$PWD/FeatureModel.h \
    $$PWD/FeatureWidget.h \
    $$PWD/Dialogs/ImportASCIIDialog.h \
    $$PWD/Graphic.h \
    $$PWD/PictureGraphic.h \
    $$PWD/QJsonModel/qjsonmodel.h \
    $$PWD/QJsonModel/qjsonview.h \
    $$PWD/Delegate.h \
#    $$PWD/TableView.h


SOURCES += \
    $$PWD/ApplicationWindow.cpp \
    $$PWD/CollectWidget.cpp \
#    $$PWD/CollectWindow.cpp \
    $$PWD/Commands.cpp \
    $$PWD/Dialogs/FindDialog.cpp \
    $$PWD/Dialogs/GraphicPropertiesDialog.cpp \
    $$PWD/Dialogs/InsertRCDialog.cpp \
    $$PWD/Dialogs/MySpinBox.cpp \
    $$PWD/Dialogs/NewFeatureDialog.cpp \
#    $$PWD/Dialogs/SortDialog.cpp \
    $$PWD/FeatureModel.cpp \
    $$PWD/FeatureWidget.cpp \
    $$PWD/Dialogs/ImportASCIIDialog.cpp \
    $$PWD/Graphic.cpp \
    $$PWD/PictureGraphic.cpp \
    $$PWD/QJsonModel/qjsonmodel.cpp \
    $$PWD/QJsonModel/qjsonview.cpp \
    $$PWD/Delegate.cpp \
#    $$PWD/TableView.cpp

FORMS += \
    $$PWD/Dialogs/FindDialog.ui \
    $$PWD/Dialogs/GraphicPropertiesDialog.ui \
    $$PWD/Dialogs/InsertRCDialog.ui \
    $$PWD/Dialogs/NewFeatureDialog.ui \
    $$PWD/Dialogs/SortDialog.ui \
    $$PWD/FeatureInfo.ui

RESOURCES += \
        $$PWD/GPjsonEditor.qrc

