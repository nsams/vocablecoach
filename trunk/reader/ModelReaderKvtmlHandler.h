#ifndef MODELREADERKVTMLHANDLER_H
#define MODELREADERKVTMLHANDLER_H

#include <QObject>
#include <QXmlDefaultHandler>
#include <QString>
#include <QHash>
#include <QMap>
class VocableListModel;
class Vocable;
class QUndoStack;
class QUndoCommand;

class ModelReaderKvtmlHandler : public QXmlDefaultHandler
{
public:
    ModelReaderKvtmlHandler(VocableListModel* model, QUndoCommand* importCommand);

	bool startElement(const QString & /* namespaceURI */,
			const QString & /* localName */,
			const QString &qName,
			const QXmlAttributes &attributes);
	bool endElement(const QString & /* namespaceURI */,
			const QString & /* localName */,
			const QString &qName);
	bool characters(const QString &str);
	bool fatalError(const QXmlParseException &exception);
	QString errorString() const;  
private:
	VocableListModel* m_model;
	Vocable* m_currentVocable;
    QUndoCommand* m_importCommand;
    QHash<int, QString> m_importLessons;

	bool metKvtmlTag;
    QString currentTag;
    QString currentLanguageType;
    QMap<QString, QVariant> currentLanguageSettings;
    QString currentLanguageDict;
    int currentLessonNumber;
	QString currentText;
	QString errorStr;
};

#endif
