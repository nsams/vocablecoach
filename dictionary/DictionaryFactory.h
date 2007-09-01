//
// C++ Interface: DictionaryFactory
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICTIONARYFACTORY_H
#define DICTIONARYFACTORY_H

#include <QObject>
#include <QMap>
#include <QString>

class DictionaryAbstract;

struct DictionaryName {
    DictionaryName(QString n = "", QString text = "")
        : displayText(text), name(n)
    { }
    QString displayText;
    QString name;
};

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class DictionaryFactory
{
public:
    static DictionaryAbstract* dictionaryInstance(const QString& type, const QMap<QString, QVariant>& settings, QObject* parent=0);
    static QList<DictionaryName> dictionaries();
};

#endif
