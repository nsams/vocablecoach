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
#include <QFile>
#include <QMessageBox>
#include "VocableListModel.h"
#include <QTextStream>

ModelReaderCsv::ModelReaderCsv(const QString& filename)
    : ModelReaderAbstract(filename)
{
}

bool ModelReaderCsv::read(VocableListModel* model)
{
    QFile file(m_fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, QObject::tr("VocableCoach"), QObject::tr("Can't open file"));
        return false;
    }

    QTextStream stream(&file);
    stream.setAutoDetectUnicode(true);
    QString line;
    do {
        line = stream.readLine();
        ModelReaderCsv::addLine(model, line);
    } while (!line.isNull());

    return true;
}

Vocable* ModelReaderCsv::addLine(VocableListModel* model, QString line, int position)
{
    QStringList parts = line.split("\t");
    Vocable* voc = new Vocable(model);
    if (position == 0) {
        model->appendVocable(voc);
    } else {
        model->insertVocable(position, voc);
    }
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


