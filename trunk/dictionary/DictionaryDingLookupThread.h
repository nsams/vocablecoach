//
// C++ Interface: DictionaryDingLookupThread
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICTIONARYDINGLOOKUPTHREAD_H
#define DICTIONARYDINGLOOKUPTHREAD_H

#include <QThread>
#include <QString>
#include <QPair>
#include <QList>
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class DictionaryDingLookupThread : public QThread
{
Q_OBJECT
public:
    DictionaryDingLookupThread(const QString& word, QObject *parent = 0);
    ~DictionaryDingLookupThread();
    QList<QPair<QString, QString> > results();

protected:
    void run();

private:
    QString m_word;
    QList<QPair<QString, QString> > m_results;
};

#endif
