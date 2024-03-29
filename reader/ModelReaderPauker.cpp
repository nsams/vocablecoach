//
// C++ Implementation: VocableListReader
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ModelReaderPauker.h"
#include "VocableListModel.h"
#include "ModelReaderPaukerHandler.h"
#include <QFile>
#include <QMessageBox>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <stdlib.h>
#include <zlib.h>
#include <QUndoCommand>

ModelReaderPauker::ModelReaderPauker(const QString& filename)
    : ModelReaderAbstract(filename)
{
}

bool ModelReaderPauker::read(VocableListModel* model, QUndoStack* undoStack)
{
    gzFile file;
    file = gzopen (m_fileName.toUtf8().data(), "rb");
    if(!file) {
        QMessageBox::critical(0, QObject::tr("import"), QObject::tr("Can't open file"));
        return false;
    }
    QByteArray input;
    char buffer[1024];
    while(int readBytes =  gzread (file, buffer, 1024))
    {
        input.append(QByteArray(buffer, readBytes));
    }
    gzclose(file);
	
    QXmlSimpleReader xmlReader;
    QXmlInputSource *source = new QXmlInputSource;
    source->setData(input);

    QUndoCommand* importCommand;
    if (undoStack) {
        importCommand = new QUndoCommand(QObject::tr("Import Vocables"));
    } else {
        importCommand = 0;
    }
    ModelReaderPaukerHandler *handler = new ModelReaderPaukerHandler(model, importCommand);
    xmlReader.setContentHandler(handler);
    xmlReader.setErrorHandler(handler);
	
    bool ok = xmlReader.parse(source);

    if (undoStack) undoStack->push(importCommand);
    
    delete source;
    delete handler;

    if (!ok) {
        QMessageBox::critical(0, QObject::tr("import"), QObject::tr("Invalid XML-File"));
        return false;
    }
    return true;
}

bool ModelReaderPauker::isValidFile()
{
    gzFile file;
    file = gzopen (m_fileName.toUtf8().data(), "rb");
    if(!file) {
        return false;
    }

    char buffer[1024];
    int readBytes =  gzread (file, buffer, 1024);
    QByteArray input(buffer, readBytes);
    gzclose(file);
    
    if (input.trimmed().left(5) != "<?xml") {
        return false;
    }
    if (!input.contains("<Lesson LessonFormat=\"1.6\">")) {
        return false;
    }
    return true;
}

