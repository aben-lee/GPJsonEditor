######################################################################
# Eageo ArchiTect - JsonModel component
# 
# Autogenerated using VNFComponentGen
######################################################################

include (../ComponentsExtend.pri)

DEPENDPATH += . interfaces source
INCLUDEPATH += . interfaces source
#INCLUDEPATH += ../../InterfacesExtend ../../InterfacesCore ../../Common

#INCLUDEPATH +=  ../../Common/IOData ../../Common/Interfaces


# Additional includes from the Job system
#INCLUDEPATH     += ../../Components/VTKSystem/Interfaces \
#                   ../../Components/VTKSystem/IOData \
#                   ../../Common

# Input
HEADERS += \ 
    source/JsonModel_p.h \
    source/TreeTableModel.h \
    interfaces/IDataLoader.h \
    interfaces/IDataMerger.h \
    interfaces/IJsonModel.h \
    interfaces/IJsonModelItem.h \
    source/TableModel_p.h
           

SOURCES += \ 
    source/JsonModel_p.cpp \
    source/TreeTableModel.cpp \
    source/TableModel_p.cpp

# IO Data interfaces and classes
INCLUDEPATH += interfaces iodata

HEADERS +=
           
SOURCES +=

RESOURCES +=



            