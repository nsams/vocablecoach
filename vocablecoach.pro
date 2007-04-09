# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: .
# Das Target ist eine Anwendung:  

FORMS += MainWindow.ui \
         VocableEditor.ui 
FORMS += VocableQuiz.ui \
StartQuiz.ui \
DocumentProperties.ui \
PreviewDialog.ui
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
           VocableListModelFilter.h \
           StartQuiz.h \
           DocumentProperties.h \
           PreviewDialog.h \
           PreviewLabel.h \
           ListVocablePrinter.h \
           AbstractVocablePrinter.h \
           CardVocablePrinter.h
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
           VocableListModelFilter.cpp \
           StartQuiz.cpp \
           DocumentProperties.cpp \
           PreviewDialog.cpp \
           PreviewLabel.cpp \
           ListVocablePrinter.cpp \
           AbstractVocablePrinter.cpp \
           CardVocablePrinter.cpp
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

INCLUDEPATH += ./3rdparty/zlib
SOURCES+= \
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
