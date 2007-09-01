//
// C++ Interface: DictionaryDictCc
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICTIONARYDICTCC_H
#define DICTIONARYDICTCC_H

#include <dictionary/DictionaryHttpAbstract.h>

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class DictionaryDictCc : public DictionaryHttpAbstract
{
Q_OBJECT
public:
    DictionaryDictCc(const QMap<QString, QVariant>& settings, QObject *parent);
    void lookupWord(const QString& word);
    static QString dictionaryName() { return tr("dict.cc"); }
    QPair<QString, QString> headerText();

private Q_SLOTS:
    void processData(bool error);

protected:
    QString host();

};

#endif
