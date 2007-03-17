//
// C++ Implementation: VocableListReader
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "VocableListReader.h"
#include "VocableListModel.h"
#include "VocableListReaderHandler.h"
#include <QFile>
#include <QMessageBox>
#include <QXmlSimpleReader>
#include <QXmlInputSource>

VocableListReader::VocableListReader(const QString& fileName)
	: m_fileName(fileName)
{
}

bool VocableListReader::read(VocableListModel* model)
{
	QFile file(m_fileName);
	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(0, QObject::tr("VocableCoach"), QObject::tr("Can't open file"));
		return false;
	}
	
	QXmlSimpleReader xmlReader;
	QXmlInputSource *source = new QXmlInputSource(&file);

	model->clearVocables();

	VocableListReaderHandler *handler = new VocableListReaderHandler(model);
	xmlReader.setContentHandler(handler);
	xmlReader.setErrorHandler(handler);
	
	bool ok = xmlReader.parse(source);

	if (!ok) {
		QMessageBox::critical(0, QObject::tr("VocableCoach"), QObject::tr("Invalid XML-File"));
		model->clearVocables();
		return false;
	}
	return true;
}
