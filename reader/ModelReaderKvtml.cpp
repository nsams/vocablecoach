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
#include "ModelReaderKvtml.h"
#include "../VocableListModel.h"
#include "ModelReaderKvtmlHandler.h"
#include <QFile>
#include <QMessageBox>
#include <QXmlSimpleReader>
#include <QXmlInputSource>

ModelReaderKvtml::ModelReaderKvtml(const QString& filename)
    : ModelReaderAbstract(filename)
{
}

bool ModelReaderKvtml::read(VocableListModel* model)
{
    QFile file(m_fileName);
	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(0, QObject::tr("VocableCoach"), QObject::tr("Can't open file"));
		return false;
	}
	
	QXmlSimpleReader xmlReader;
	QXmlInputSource *source = new QXmlInputSource(&file);

    ModelReaderKvtmlHandler *handler = new ModelReaderKvtmlHandler(model);
	xmlReader.setContentHandler(handler);
	xmlReader.setErrorHandler(handler);
	
	bool ok = xmlReader.parse(source);

	if (!ok) {
		QMessageBox::critical(0, QObject::tr("VocableCoach"), QObject::tr("Invalid XML-File"));
		return false;
	}
	return true;
}

bool ModelReaderKvtml::isValidFile()
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
