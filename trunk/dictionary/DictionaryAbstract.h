//
// C++ Interface: DictionaryAbstract
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICTIONARYABSTRACT_H
#define DICTIONARYABSTRACT_H
#include <QObject>
#include <QList>
#include <QPair>
#include <QString>
#include <QVariant>
#include <QMap>

class QDialog;
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class DictionaryAbstract : public QObject
{
    Q_OBJECT
public:
    DictionaryAbstract(const QMap<QString, QVariant>& settings, QObject* parent=0);
    virtual QList<QPair<QString, QString> > results();
    virtual void lookupWord(const QString& word);
    virtual bool hasSettings() {
        return false;
    }
    virtual QMap<QString, QVariant> editSettings(QWidget* parent) {
        Q_UNUSED(parent);
        return m_settings;
    }
    virtual QPair<QString, QString> headerText() = 0;

Q_SIGNALS:
    void done(bool error);

protected:
    QList<QPair<QString, QString> > m_results;
    QMap<QString, QVariant> m_settings;
};

#endif
