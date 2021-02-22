######################################################################
# Eageo Architect - CustomJobms component
# 
######################################################################

#include (../ComponentsExtend.pri)

CONFIG += ECFGui
CONFIG -= app_bundle
TEMPLATE    = app

QT += printsupport

DESTDIR     = $$PWD/../../Binary/GeoArch
TARGET      = TableEditor

# Input
include     ($$PWD/GPjsonEditor.pri)

SOURCES += \
    main_customview.cpp

FORMS +=

HEADERS +=

