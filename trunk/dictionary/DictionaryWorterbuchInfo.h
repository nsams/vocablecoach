//
// C++ Interface: DictionaryWorterbuchInfo
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICTIONARYWORTERBUCHINFO_H
#define DICTIONARYWORTERBUCHINFO_H

#include <dictionary/DictionaryHttpAbstract.h>

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class DictionaryWorterbuchInfo : public DictionaryHttpAbstract
{
Q_OBJECT
public:
    DictionaryWorterbuchInfo(const QMap<QString, QVariant>& settings, QObject *parent);
    void lookupWord(const QString& word);
    static QString dictionaryName() { return tr("worterbuch.info"); }
    QPair<QString, QString> headerText();

private Q_SLOTS:
    void processData(bool error);

protected:
    QString host();
};

#endif
