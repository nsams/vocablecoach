# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: .
# Das Target ist eine Anwendung:  

FORMS += MainWindow.ui \
         VocableEditor.ui 
FORMS += VocableQuiz.ui
HEADERS += MainWindow.h \
           Vocable.h \
           VocableListModel.h \
           ImportPauker.h \
           VocableListWriter.h \
           VocableEditor.h  \
           VocableQuiz.h \
           VocableListReader.h \
           VocableListReaderHandler.h \
           VocableEditorTextEdit.h \
           VocableListModelFilter.h
SOURCES += main.cpp \
           MainWindow.cpp \
           VocableListModel.cpp \
           ImportPauker.cpp \
           VocableListWriter.cpp \
           VocableEditor.cpp  \
           VocableQuiz.cpp \
           VocableListReader.cpp \
           VocableListReaderHandler.cpp \
           Vocable.cpp \
           VocableEditorTextEdit.cpp \
           VocableListModelFilter.cpp
CONFIG += debug \
qt \
warn_on
TEMPLATE = app
TARGET +=
DEPENDPATH += .
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = ui
DISTFILES += ../pixelreiter/todo.txt

QT += core \
gui \
xml \
network
INCLUDEPATH += .

