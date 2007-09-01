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
    DictionaryUniLeipzig(const QMap<QString, QVariant>& settings, QObject *parent);
    void lookupWord(const QString& word);
    static QString dictionaryName() { return tr("dict.uni-leizpig.de"); }
    QPair<QString, QString> headerText();

private Q_SLOTS:
    void processData(bool error);

protected:
    QString host();

};

#endif
