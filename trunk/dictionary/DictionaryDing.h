//
// C++ Interface: DictionaryDing
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICTIONARYDING_H
#define DICTIONARYDING_H

#include "dictionary/DictionaryAbstract.h"
class DictionaryDingLookupThread;
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class DictionaryDing : public DictionaryAbstract
{
Q_OBJECT
public:
    DictionaryDing(const QMap<QString, QVariant>& settings, QObject *parent = 0);

    void lookupWord(const QString& word);
    static QString dictionaryName() { return tr("ding (local)"); }
    QPair<QString, QString> headerText();
private:
    DictionaryDingLookupThread* m_thread;

private Q_SLOTS:
    void processData();
};

#endif
