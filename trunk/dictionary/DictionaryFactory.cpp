//
// C++ Implementation: DictionaryFactory
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryFactory.h"
#include "dictionary/DictionaryAbstract.h"
#include "dictionary/DictionaryDing.h"
#include "dictionary/DictionaryLeo.h"
#include "dictionary/DictionaryDictCc.h"
#include "dictionary/DictionaryWorterbuchInfo.h"
#include "dictionary/DictionaryUniLeipzig.h"


QList<DictionaryName> DictionaryFactory::dictionaries()
{
    QList<DictionaryName> ret;
    ret << DictionaryName("none", QObject::tr("none"));
    ret << DictionaryName("Ding", DictionaryDing::dictionaryName());
    ret << DictionaryName("Leo", DictionaryLeo::dictionaryName());
    ret << DictionaryName("DictCc", DictionaryDictCc::dictionaryName());
    ret << DictionaryName("WoerterbuchInfo", DictionaryWorterbuchInfo::dictionaryName());
    ret << DictionaryName("UniLeipzig", DictionaryUniLeipzig::dictionaryName());
    return ret;
}

DictionaryAbstract* DictionaryFactory::dictionaryInstance(const QString& type, const QMap<QString, QVariant>& settings, QObject* parent)
{
    DictionaryAbstract* ret;
    if (type == "Ding") {
        ret = new DictionaryDing(settings, parent);
    } else if (type == "Leo") {
        ret = new DictionaryLeo(settings, parent);
    } else if (type == "DictCc") {
        ret = new DictionaryDictCc(settings, parent);
    } else if (type == "WoerterbuchInfo") {
        ret = new DictionaryWorterbuchInfo(settings, parent);
    } else if (type == "UniLeipzig") {
        ret = new DictionaryUniLeipzig(settings, parent);
    } else {
        ret = 0;
    }
    return ret;
}
