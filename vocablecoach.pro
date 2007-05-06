# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: .
# Das Target ist eine Anwendung:  


FORMS += MainWindow.ui \
         VocableEditor.ui \
         VocableQuiz.ui \
         StartQuiz.ui \
         DocumentProperties.ui \
         PreviewDialog.ui

HEADERS += MainWindow.h \
           Vocable.h \
           VocableListModel.h \
           VocableEditor.h  \
           VocableQuiz.h \
           VocableEditorTextEdit.h \
           VocableListModelFilter.h \
           StartQuiz.h \
           DocumentProperties.h \
           PreviewDialog.h \
           PreviewLabel.h \
           ListVocablePrinter.h \
           AbstractVocablePrinter.h \
           CardVocablePrinter.h \
           reader/ModelReaderAbstract.h \
           reader/ModelReaderKvtml.h \
           reader/ModelReaderKvtmlHandler.h \
           reader/ModelReaderPauker.h \
           reader/ModelReaderPaukerHandler.h \
           writer/ModelWriterAbstract.h \
           writer/ModelWriterKvtml.h \
           writer/ModelWriterCsv.h \
           reader/ModelReaderCsv.h \
           command/CommandDelete.h \
           command/CommandAdd.h \
           command/CommandEdit.h \
           command/CommandEditProperties.h \
           command/CommandQuizAnswer.h
SOURCES += main.cpp \
           MainWindow.cpp \
           VocableListModel.cpp \
           VocableEditor.cpp  \
           VocableQuiz.cpp \
           Vocable.cpp \
           VocableEditorTextEdit.cpp \
           VocableListModelFilter.cpp \
           StartQuiz.cpp \
           DocumentProperties.cpp \
           PreviewDialog.cpp \
           PreviewLabel.cpp \
           ListVocablePrinter.cpp \
           CardVocablePrinter.cpp \
           AbstractVocablePrinter.cpp \
           reader/ModelReaderAbstract.cpp \
           reader/ModelReaderKvtml.cpp \
           reader/ModelReaderKvtmlHandler.cpp \
           reader/ModelReaderPauker.cpp \
           reader/ModelReaderPaukerHandler.cpp \
           writer/ModelWriterAbstract.cpp \
           writer/ModelWriterKvtml.cpp \
           writer/ModelWriterCsv.cpp \
           reader/ModelReaderCsv.cpp \
           command/CommandDelete.cpp \
           command/CommandAdd.cpp \
           command/CommandEdit.cpp \
           command/CommandEditProperties.cpp \
           command/CommandQuizAnswer.cpp
CONFIG += debug \
qt \
warn_on
TEMPLATE = app
TARGET +=
DEPENDPATH += .
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = ui
DISTFILES += COPYING
QT += core \
gui \
xml \
network
INCLUDEPATH += .

TRANSLATIONS = translation/de.ts

