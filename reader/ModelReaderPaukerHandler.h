#ifndef MODELREADERPAUKERHANDLER_H
#define MODELREADERPAUKERHANDLER_H
#include <QString>
#include <QXmlDefaultHandler>
class VocableListModel;
class Vocable;
class QUndoCommand;

class ModelReaderPaukerHandler : public QXmlDefaultHandler
{
public:
    ModelReaderPaukerHandler(VocableListModel* model, QUndoCommand* undoCommand);

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    QString errorString() const;

private:
    bool metLessonTag;
    int currentBox;
    Vocable* currentVocable;
    int currentSide;
    
    QString currentText;
    QString errorStr;

	VocableListModel* m_model;
    QUndoCommand* m_importCommand;
};

#endif
