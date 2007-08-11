//
// C++ Interface: DictionaryUniLeipzig
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICTIONARYUNILEIPZIG_H
#define DICTIONARYUNILEIPZIG_H

#include <dictionary/DictionaryHttpAbstract.h>

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class DictionaryUniLeipzig : public DictionaryHttpAbstract
{
Q_OBJECT
public:
    DictionaryUniLeipzig(QObject *parent = 0);
    void lookupWord(const QString& word);

private Q_SLOTS:
    void processData(bool error);

protected:
    QString host();

};

#endif
