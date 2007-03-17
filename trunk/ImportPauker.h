#ifndef IMPORTPAUKER_H
#define IMPORTPAUKER_H
#include <QString>
#include <QXmlDefaultHandler>
class VocableListModel;
class Vocable;

class ImportPauker : public QXmlDefaultHandler
{
public:
    ImportPauker(VocableListModel* model);

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    QString errorString() const;

private:
    bool metLessionTag;
    int currentBatch;
    Vocable* currentVocable;
    int currentSide;
    
    QString currentText;
    QString errorStr;

	VocableListModel* m_model;
};

#endif
