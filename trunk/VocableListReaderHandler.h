#ifndef VOCABLELISTREADERHANDLER_H
#define VOCABLELISTREADERHANDLER_H

#include <QXmlDefaultHandler>
#include <QString>
class VocableListModel;
class Vocable;

class VocableListReaderHandler : public QXmlDefaultHandler
{
public:
	VocableListReaderHandler(VocableListModel* model);

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

	bool metKvtmlTag;
	QString currentText;
	QString errorStr;
};

#endif
