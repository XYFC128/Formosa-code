QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Formosa-code
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += qscintilla2

macx {
    QMAKE_POST_LINK = install_name_tool -change libqscintilla2_qt$${QT_MAJOR_VERSION}.13.dylib $$[QT_INSTALL_LIBS]/libqscintilla2_qt$${QT_MAJOR_VERSION}.13.dylib $(TARGET)
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    editor.cpp \
    fileview.cpp

HEADERS += \
        mainwindow.h \
    editor.h \
    fileview.h
RESOURCES += \
            resource.qrc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
unix: LIBS += -L$$PWD/Scintilla/ -lqscintilla2_qt5

INCLUDEPATH += $$PWD/Scintilla
DEPENDPATH += $$PWD/Scintilla

win32: LIBS += -L$$PWD/Scintilla-win/ -lqscintilla2_qt5

INCLUDEPATH += $$PWD/Scintilla-win
DEPENDPATH += $$PWD/Scintilla-win

DISTFILES +=
