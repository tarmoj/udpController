TEMPLATE = app

QT += qml quick widgets network websockets

DESTDIR=bin #Target file directory
OBJECTS_DIR=generated_files #Intermediate object files directory
MOC_DIR=generated_files #Intermediate moc files directory

SOURCES += main.cpp \
    wsserver.cpp  

RESOURCES += qml.qrc 

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =


HEADERS += \
    wsserver.h 
    

linux: HEADERS +=  jackreader.h


macx {
    
    final.path = $$PWD
    final.commands = $$[QT_INSTALL_PREFIX]/bin/macdeployqt $$OUT_PWD/$$DESTDIR/$${TARGET}.app -qmldir=$$PWD -dmg# deployment

    INSTALLS += final 

}
