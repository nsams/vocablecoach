#include "ImportPauker.h"
#include <QObject>
#include <QMessageBox>
#include <QXmlInputSource>
#include "Vocable.h"
#include <QDebug>
#include "VocableListModel.h"


ImportPauker::ImportPauker(VocableListModel* model)
 : QXmlDefaultHandler()
{
	m_model = model;
    metLessonTag = false;
    currentBox = 0;
}

bool ImportPauker::startElement(const QString & /* namespaceURI */,
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
        currentVocable = new Vocable(m_model);
        int i = currentBox;
        currentVocable->setBox(i);
		m_model->appendVocable(currentVocable);
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

bool ImportPauker::endElement(const QString & /* namespaceURI */,
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

bool ImportPauker::characters(const QString &str)
{
    currentText += str;
    return true;
}
bool ImportPauker::fatalError(const QXmlParseException &exception)
{
    QMessageBox::information(0, QObject::tr("import"),
                             QObject::tr("Parse error at line %1, column %2:\n"
                                         "%3")
                             .arg(exception.lineNumber())
                             .arg(exception.columnNumber())
                             .arg(exception.message()));
    return false;
}

QString ImportPauker::errorString() const
{
    return errorStr;
}
