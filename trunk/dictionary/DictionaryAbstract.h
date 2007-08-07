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
/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class DictionaryAbstract : public QObject
{
    Q_OBJECT
public:
    DictionaryAbstract(QObject* parent=0);
    QList<QPair<QString, QString> > results();

public:
    virtual void lookupWord(const QString& word);

Q_SIGNALS:
    void done(bool error);

protected:
    QList<QPair<QString, QString> > m_results;
};

#endif
