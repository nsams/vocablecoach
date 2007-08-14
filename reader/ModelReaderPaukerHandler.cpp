#include "ModelReaderPaukerHandler.h"
#include "Vocable.h"
#include "VocableListModel.h"
#include "command/CommandAdd.h"
#include <QObject>
#include <QMessageBox>
#include <QXmlInputSource>
#include <QDebug>

ModelReaderPaukerHandler::ModelReaderPaukerHandler(VocableListModel* model, QUndoCommand* importCommand)
: QXmlDefaultHandler(), m_model(model), m_importCommand(importCommand)
{
    metLessonTag = false;
    currentBox = 0;
}

bool ModelReaderPaukerHandler::startElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString &qName,
                               const QXmlAttributes &attributes)
{
    if (!metLessonTag && qName != "Lesson") {
        errorStr = QObject::tr("The file is not an Pauker file. (%1)").arg(qName);
        return false;
	}
	
	if (qName == "Lesson") {
        QString version = attributes.value("LessonFormat");
        if (!version.isEmpty() && version != "1.6") {
            errorStr = QObject::tr("The file is not an Pauker version 1.6 file.");
            return false;
        }
        metLessonTag = true; 
    } else if (qName == "Description") {
        //todo: save description: m_model->setTitle();
    } else if (qName == "Batch") {
    	currentBox++;
    } else if (qName == "Card") {
        currentVocable = m_model->createVocable();
        int i = currentBox;
        currentVocable->setBox(Vocable::NativeToForeign, i);
        if (m_importCommand) {
            new CommandAdd(m_model, currentVocable, -1, m_importCommand);
        } else {
            m_model->appendVocable(currentVocable);
        }
        currentSide = 0;
	} else if (qName == "FrontSide") {
		currentSide = 1;
	} else if (qName == "ReverseSide") {
		currentSide = 2;
	} else if (qName == "Text") {
		currentText.clear();
	}
    return true;
}

bool ModelReaderPaukerHandler::endElement(const QString & /* namespaceURI */,
                             const QString & /* localName */,
                             const QString &qName)
{
    if (qName == "Text") {
    	if(currentSide && currentBox && currentVocable) {
    		if(currentSide==1) currentVocable->setNative(currentText);
    		else if(currentSide==2) currentVocable->setForeign(currentText);
    	}
    }
    return true;
}

bool ModelReaderPaukerHandler::characters(const QString &str)
{
    currentText += str;
    return true;
}
bool ModelReaderPaukerHandler::fatalError(const QXmlParseException &exception)
{
    QMessageBox::information(0, QObject::tr("import"),
                             QObject::tr("Parse error at line %1, column %2:\n"
                                         "%3")
                             .arg(exception.lineNumber())
                             .arg(exception.columnNumber())
                             .arg(exception.message()));
    return false;
}

QString ModelReaderPaukerHandler::errorString() const
{
    return errorStr;
}
