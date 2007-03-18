//
// C++ Implementation: VocableListReaderHandler
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "VocableListReaderHandler.h"
#include "VocableListModel.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

VocableListReaderHandler::VocableListReaderHandler(VocableListModel* model)
 : QXmlDefaultHandler()
{
	m_model = model;
    metKvtmlTag = false;
}
bool VocableListReaderHandler::startElement(const QString & /* namespaceURI */,
								const QString & /* localName */,
								const QString &qName,
								const QXmlAttributes &attributes)
{
	if (!metKvtmlTag && qName != "kvtml") {
		errorStr = QObject::tr("The file is not an KVTML file. (%1)").arg(qName);
		return false;
	}

	if (qName == "kvtml") {
		//todo: save title: attributes.value("title");
		metKvtmlTag = true; 
	} else if (qName == "e") {
		m_currentVocable = new Vocable(m_model);
		m_model->appendVocable(m_currentVocable);
		//todo: m = member von lession nr: attributes.value("m");
		m_currentVocable->setBox(attributes.value("box").toInt());
        m_currentVocable->setLession(attributes.value("lession"));
	} else if (qName == "o") {
		currentText.clear();
	} else if (qName == "t") {
		QStringList lastQueryList = QString(attributes.value("d")).split(";");
		if(lastQueryList[0].toUInt() > 0) {
			QDateTime lastQuery = QDateTime::fromTime_t(lastQueryList[0].toUInt());
			m_currentVocable->setLastQuery(lastQuery);
		}
		currentText.clear();
	}
	return true;
}

bool VocableListReaderHandler::endElement(const QString & /* namespaceURI */,
							  const QString & /* localName */,
							  const QString &qName)
{
	if (qName == "o" && m_currentVocable) {
		m_currentVocable->setForeign(currentText);
	} else if(qName == "t" && m_currentVocable) {
		m_currentVocable->setNative(currentText);
	}
	return true;
}
bool VocableListReaderHandler::characters(const QString &str)
{
	currentText += str;
	return true;
}
bool VocableListReaderHandler::fatalError(const QXmlParseException &exception)
{
	QMessageBox::information(0, QObject::tr("VocableCoach"),
							 QObject::tr("Parse error at line %1, column %2:\n"
									 "%3")
									 .arg(exception.lineNumber())
									 .arg(exception.columnNumber())
									 .arg(exception.message()));
	return false;
}

QString VocableListReaderHandler::errorString() const
{
	return errorStr;
}

