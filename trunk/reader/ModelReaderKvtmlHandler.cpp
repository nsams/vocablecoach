//
// C++ Implementation: ModelReaderKvtmlHandler
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ModelReaderKvtmlHandler.h"
#include "VocableListModel.h"
#include "command/CommandAdd.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QUndoStack>
ModelReaderKvtmlHandler::ModelReaderKvtmlHandler(VocableListModel* model, QUndoCommand* importCommand)
    : QXmlDefaultHandler(), m_model(model), m_importCommand(importCommand)
{
    metKvtmlTag = false;
    inLessonTag = false;
}
bool ModelReaderKvtmlHandler::startElement(const QString & /* namespaceURI */,
								const QString & /* localName */,
								const QString &qName,
								const QXmlAttributes &attributes)
{
	if (!metKvtmlTag && qName != "kvtml") {
		errorStr = QObject::tr("The file is not an KVTML file. (%1)").arg(qName);
		return false;
	}
    if (qName == "kvtml") {
        if (!m_importCommand) {
            //only set when opening, not when importing (where we have an undoStack)
            m_model->setTitle(attributes.value("title"));
            m_model->setAuthors(attributes.value("author"));
            if(!attributes.value("nativeLanguage").isEmpty()) m_model->setForeignLanguage(attributes.value("foreignLanguage"));
            if(!attributes.value("nativeLanguage").isEmpty()) m_model->setNativeLanguage(attributes.value("nativeLanguage"));
        }
		metKvtmlTag = true; 
    } else if (qName == "lesson") {
        inLessonTag = true;
    } else if (inLessonTag && qName == "desc") {
        currentLessonNumber = attributes.value("no").toInt();
        currentText.clear();
	} else if (qName == "e") {
        m_currentVocable = m_model->createVocable();
        if (m_importCommand) {
            new CommandAdd(m_model, m_currentVocable, -1, m_importCommand);
        } else {
            m_model->appendVocable(m_currentVocable);
        }
		m_currentVocable->setBox(attributes.value("box").toInt());
        if(!attributes.value("m").isEmpty()) {
            m_currentVocable->setLessonNumber(attributes.value("m").toInt());
        } else if(!attributes.value("lesson").isEmpty()) {
            m_currentVocable->setLesson(attributes.value("lesson"));
        } else if(!attributes.value("lession").isEmpty()) { //deprecated
            m_currentVocable->setLesson(attributes.value("lession"));
        }
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

bool ModelReaderKvtmlHandler::endElement(const QString & /* namespaceURI */,
							  const QString & /* localName */,
							  const QString &qName)
{
    if (inLessonTag && qName == "desc" && !m_importCommand) {
        //only set when opening, not when importing (where we have an undoStack)
        m_model->insertLesson(currentLessonNumber, currentText.trimmed());
    } else if (qName == "lesson") {
        inLessonTag = false;
    } else if (qName == "o" && m_currentVocable) {
		m_currentVocable->setForeign(currentText);
	} else if(qName == "t" && m_currentVocable) {
		m_currentVocable->setNative(currentText);
	}
	return true;
}
bool ModelReaderKvtmlHandler::characters(const QString &str)
{
	currentText += str;
	return true;
}
bool ModelReaderKvtmlHandler::fatalError(const QXmlParseException &exception)
{
	QMessageBox::information(0, QObject::tr("VocableCoach"),
							 QObject::tr("Parse error at line %1, column %2:\n"
									 "%3")
									 .arg(exception.lineNumber())
									 .arg(exception.columnNumber())
									 .arg(exception.message()));
	return false;
}

QString ModelReaderKvtmlHandler::errorString() const
{
	return errorStr;
}

