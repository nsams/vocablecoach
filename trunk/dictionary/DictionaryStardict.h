//
// C++ Interface: DictionaryStardict
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
// Based on QStarDict Copyright (C) 2007 Alexander Rodin                                           *
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICTIONARYSTARDICT_H
#define DICTIONARYSTARDICT_H

#include "dictionary/DictionaryAbstract.h"
#include <QStringList>
class Libs;

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class DictionaryStardict : public DictionaryAbstract
{
Q_OBJECT
public:
    DictionaryStardict(const QMap<QString, QVariant>& settings, QObject *parent = 0);
    ~DictionaryStardict();


    void lookupWord(const QString& word);
    static QString dictionaryName() { return tr("Stardict (local)"); }
    bool hasSettings() {
        return true;
    }
    QMap<QString, QVariant> editSettings(QWidget* parent);
    QPair<QString, QString> headerText();

private:
    class SearchResult
    {
        public:
            QString def;
            QString exp;

            SearchResult()
            { }
            SearchResult(const QString &_def, const QString &_exp)
            : def(_def), exp(_exp)
            { }
    };
    typedef QList<SearchResult> SearchResultList;

    QStringList m_dictDirs;
    Libs *m_sdLibs;
    void _loadDict();
    QString _whereDict(const QString &name, const QString &path);
    void _lookupWithFuzzy(const std::string &str, SearchResultList &resultList);
    void _simpleLookup(const std::string &str, SearchResultList &resultList);
    QString _parse_data(const char *data);
    QStringList _avialableDicts() const;
    QStringList _findDicts(const QString &dir) const;

    static const int MaxFuzzy;
};


#endif
