# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: .
# Das Target ist eine Anwendung:  


FORMS += MainWindow.ui \
         VocableEditor.ui \
         VocableQuiz.ui \
         StartQuiz.ui \
         DocumentProperties.ui \
         PreviewDialog.ui \
         VocableList.ui \
         StatisticsWidget.ui

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
           VocableList.h \
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
           command/CommandQuizAnswer.h \
           command/CommandModifyLesson.h \
           command/CommandResetBox.h \
           StatisticsWidget.h \
           dictionary/DictionaryAbstract.h \
           dictionary/DictionaryLeo.h \
           dictionary/DictionaryHttpAbstract.h \
           dictionary/DictionaryDictCc.h \
           dictionary/DictionaryWorterbuchInfo.h \
           dictionary/DictionaryUniLeipzig.h \
           dictionary/DictionaryDing.h \
           dictionary/DictionaryDingResultItem.h \
 dictionary/DictionaryDingLookupThread.h
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
           VocableList.cpp \
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
           command/CommandQuizAnswer.cpp \
           command/CommandModifyLesson.cpp \
           command/CommandResetBox.cpp \
           StatisticsWidget.cpp \
           dictionary/DictionaryAbstract.cpp \
           dictionary/DictionaryLeo.cpp \
           dictionary/DictionaryHttpAbstract.cpp \
           dictionary/DictionaryDictCc.cpp \
           dictionary/DictionaryWorterbuchInfo.cpp \
           dictionary/DictionaryUniLeipzig.cpp \
           dictionary/DictionaryDing.cpp \
           dictionary/DictionaryDingResultItem.cpp \
 dictionary/DictionaryDingLookupThread.cpp

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

TRANSLATIONS = translations/de.ts

win32 {
    INCLUDEPATH += ./3rdparty/zlib
    SOURCES += \
        ./3rdparty/zlib/adler32.c \
        ./3rdparty/zlib/compress.c \
        ./3rdparty/zlib/crc32.c \
        ./3rdparty/zlib/deflate.c \
        ./3rdparty/zlib/gzio.c \
        ./3rdparty/zlib/inffast.c \
        ./3rdparty/zlib/inflate.c \
        ./3rdparty/zlib/inftrees.c \
        ./3rdparty/zlib/trees.c \
        ./3rdparty/zlib/uncompr.c \
        ./3rdparty/zlib/zutil.c
}

RESOURCES += icons.qrc
