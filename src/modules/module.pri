include(../../../config.pri)

CONFIG   += precompile_header thread dll
LANGUAGE  = C++
TEMPLATE = lib

QT += core \
    gui \
    qt3support
    
DEFINES -= UNICODE

CONFIG(debug, debug|release)
{
	DESTDIR = ../../../bin/debug/modules/
	LIBS += -L../../../bin/debug/
} else {
	DESTDIR = ../../../bin/release/modules/
	LIBS += -L../../../bin/release/
}
    
INCLUDEPATH += ../../kvilib/tal/ \
    ../../kvilib/config/ \
    ../../kvilib/core/ \
    ../../kvilib/ext/ \
    ../../kvilib/file/ \
    ../../kvilib/irc/ \
    ../../kvilib/net/ \
    ../../kvilib/system/
          
  
INCLUDEPATH += ../../kvirc/kernel/ \
    ../../kvirc/kvs/ \
    ../../kvirc/module/ \
    ../../kvirc/sparser/ \
    ../../kvirc/ui/
    

LIBS += -lkvilib
	      
mac {
	LIBS += -Wl,-undefined,dynamic_lookup
}

win32 { 
	LIBS += -lkvirc  
	RC_FILE = ../../../data/resources/commctrl.rc
}

unix:LIBS += -lpthread -lssl -lcrypto -lz
unix:DEFINES += COMPILE_THREADS_USE_POSIX COMPILE_USE_QT4


target.path = ../../../bin/image/modules/
INSTALLS += target 

CONFIG(debug, debug|release) {
	MOC_DIR      = ../../../build/modules/$$replace(TARGET,kvi,)/debug/moc
	OBJECTS_DIR  = ../../../build/modules/$$replace(TARGET,kvi,)/debug/obj
} else {
	MOC_DIR      = ../../../build/modules/$$replace(TARGET,kvi,)/release/moc
	OBJECTS_DIR  = ../../../build/modules/$$replace(TARGET,kvi,)/release/obj
}

