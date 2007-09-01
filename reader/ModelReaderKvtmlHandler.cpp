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
    currentTag = "";
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
            if(!attributes.value("foreignLanguage").isEmpty()) m_model->setForeignLanguage(attributes.value("foreignLanguage"));
            if(!attributes.value("nativeLanguage").isEmpty()) m_model->setNativeLanguage(attributes.value("nativeLanguage"));
        }
		metKvtmlTag = true; 
    } else if (qName == "languages") {
        currentTag = qName;
    } else if (currentTag == "languages" && qName == "language") {
        currentLanguageType = attributes.value("type");
        currentLanguageDict = attributes.value("dict");
        currentText.clear();
    } else if (currentTag == "languages" && qName == "dictSetting") {
        if (attributes.value("key") != "") {
            currentLanguageSettings[attributes.value("key")] = attributes.value("value");
        }
    } else if (qName == "lesson") {
        currentTag = qName;
        if (!m_importCommand) {
            //only set when opening, not when importing (where we have an undoStack)
            if (attributes.value("width").toInt() > 0) {
                m_model->setLessonColumnWidth(attributes.value("width").toInt());
            }
        }
    } else if (currentTag == "lesson" && qName == "desc") {
        currentLessonNumber = attributes.value("no").toInt();
        currentText.clear();
	} else if (qName == "e") {
        m_currentVocable = m_model->createVocable();
        if (m_importCommand) {
            new CommandAdd(m_model, m_currentVocable, -1, m_importCommand);
        } else {
            m_model->appendVocable(m_currentVocable);
        }
        if (!attributes.value("box").isEmpty()) {
             //deprecated, <t g="" is used instead (which is kvtml standard)
            m_currentVocable->setBox(Vocable::NativeToForeign, attributes.value("box").toInt());
        }
        if(!attributes.value("m").isEmpty()) {
            if (m_importCommand) {
                if (m_importLessons.contains(attributes.value("m").toInt())) {
                    m_currentVocable->setLesson(m_importLessons.value(attributes.value("m").toInt()));
                }
            } else {
                m_currentVocable->setLessonNumber(attributes.value("m").toInt());
            }
        } else if(!attributes.value("lesson").isEmpty()) {
            m_currentVocable->setLesson(attributes.value("lesson"));
        } else if(!attributes.value("lession").isEmpty()) { //deprecated
            m_currentVocable->setLesson(attributes.value("lession"));
        }
	} else if (qName == "o") {
        if (!m_importCommand) {
            //only set when opening, not when importing (where we have an undoStack)
            if (attributes.value("width").toInt() > 0) {
                m_model->setNativeColumnWidth(attributes.value("width").toInt());
            }
        }
        currentText.clear();
	} else if (qName == "t") {
        if (!m_importCommand) {
            //only set when opening, not when importing (where we have an undoStack)
            if (attributes.value("width").toInt() > 0) {
                m_model->setForeignColumnWidth(attributes.value("width").toInt());
            }
        }
        QStringList valList = QString(attributes.value("d")).split(";"); //last query
        if(valList[0].toUInt() > 0) {
            QDateTime lastQuery = QDateTime::fromTime_t(valList[0].toUInt());
            m_currentVocable->setLastQuery(Vocable::NativeToForeign, lastQuery);
		}
        if(valList.count() > 1 && valList[1].toUInt() > 0) {
            QDateTime lastQuery = QDateTime::fromTime_t(valList[1].toUInt());
            m_currentVocable->setLastQuery(Vocable::ForeignToNative, lastQuery);
        }
        valList = QString(attributes.value("c")).split(";"); //query-count
        if(valList[0].toInt() > 0) {
            m_currentVocable->setQueryCount(Vocable::NativeToForeign, valList[0].toInt());
        }
        if(valList.count() > 1 && valList[1].toInt() > 0) {
            m_currentVocable->setQueryCount(Vocable::ForeignToNative, valList[1].toInt());
        }
        valList = QString(attributes.value("b")).split(";"); //bad-count
        if(valList[0].toInt() > 0) {
            m_currentVocable->setBadCount(Vocable::NativeToForeign, valList[0].toInt());
        }
        if(valList.count() > 1 && valList[1].toInt() > 0) {
            m_currentVocable->setBadCount(Vocable::ForeignToNative, valList[1].toInt());
        }
        valList = QString(attributes.value("g")).split(";"); //grade
        if(valList[0].toInt() > 0) {
            m_currentVocable->setBox(Vocable::NativeToForeign, valList[0].toInt());
        }
        if(valList.count() > 1 && valList[1].toInt() > 0) {
            m_currentVocable->setBox(Vocable::ForeignToNative, valList[1].toInt());
        }
        currentText.clear();
	}
	return true;
}

bool ModelReaderKvtmlHandler::endElement(const QString & /* namespaceURI */,
							  const QString & /* localName */,
							  const QString &qName)
{
    if (currentTag == "languages" && qName == "language") {
        if (!m_importCommand) {
            //only set when opening, not when importing (where we have an undoStack)
            if (currentLanguageType == "native") {
                m_model->setNativeLanguageSettings(currentLanguageSettings);
                m_model->setNativeLanguage(currentText.trimmed());
                m_model->setNativeLanguageDict(currentLanguageDict);
            } else if (currentLanguageType == "foreign") {
                m_model->setForeignLanguageSettings(currentLanguageSettings);
                m_model->setForeignLanguage(currentText.trimmed());
                m_model->setForeignLanguageDict(currentLanguageDict);
            }
        }
    } else if (qName == "languages") {
        currentTag = "";
    } else if (currentTag == "lesson" && qName == "desc") {
        if (!m_importCommand) {
            //only set when opening, not when importing (where we have an undoStack)
            m_model->insertLesson(currentLessonNumber, currentText.trimmed());
        } else {
            m_importLessons[currentLessonNumber] = currentText.trimmed();
        }
    } else if (qName == "lesson") {
        currentTag = "";
    } else if (qName == "o" && m_currentVocable) {
        m_currentVocable->setNative(currentText);
	} else if(qName == "t" && m_currentVocable) {
        m_currentVocable->setForeign(currentText);
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

