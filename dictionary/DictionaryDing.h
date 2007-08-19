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
    DictionaryDing(QObject *parent = 0);

    ~DictionaryDing();
    void lookupWord(const QString& word);
private:
    DictionaryDingLookupThread* m_thread;

private Q_SLOTS:
    void processData();
};

#endif
