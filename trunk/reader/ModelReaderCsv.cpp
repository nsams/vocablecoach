//
// C++ Implementation: ModelReaderCsv
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ModelReaderCsv.h"
#include "VocableListModel.h"
#include "command/CommandAdd.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QUndoCommand>

ModelReaderCsv::ModelReaderCsv(const QString& filename)
    : ModelReaderAbstract(filename)
{
}

bool ModelReaderCsv::read(VocableListModel* model, QUndoStack* undoStack)
{
    QFile file(m_fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, QObject::tr("VocableCoach"), QObject::tr("Can't open file"));
        return false;
    }
    
    QUndoCommand* importCommand;
    if (undoStack) {
        importCommand = new QUndoCommand(QObject::tr("Import Vocables"));
    }
    
    QTextStream stream(&file);
    stream.setAutoDetectUnicode(true);
    QString line;
    do {
        line = stream.readLine();
        Vocable* voc = ModelReaderCsv::parseLine(model, line);
        if (undoStack) {
            new CommandAdd(model, voc, -1, importCommand);
        } else {
            model->appendVocable(voc);
        }
    } while (!line.isNull());

    if (undoStack) undoStack->push(importCommand);

    return true;
}

Vocable* ModelReaderCsv::parseLine(VocableListModel* model, QString line)
{
    QStringList parts = line.split("\t");
    Vocable* voc = model->createVocable();
    voc->setNative(parts[0]);
    if (parts.count() > 1) voc->setForeign(parts[1]);
    if (parts.count() > 2) voc->setBox(parts[2].toInt());
    return voc;
}

bool ModelReaderCsv::isValidFile()
{
    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return false;
    }

    QString line = file.readLine();
    if (line.trimmed().left(5) != "<?xml") {
        return false;
    }
    if(file.atEnd()) return false;

    line = file.readLine();
    if (line.trimmed().left(39) != "<!DOCTYPE kvtml SYSTEM \"kvoctrain.dtd\">") {
        return false;
    }

    return true;
}


