//
// C++ Implementation: DictionaryAbstract
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryAbstract.h"
#include <QDebug>

DictionaryAbstract::DictionaryAbstract(const QMap<QString, QVariant>& settings, QObject* parent)
    : QObject(parent), m_settings(settings)
{
}

void DictionaryAbstract::lookupWord(const QString& word)
{
    Q_UNUSED(word);
    m_results.clear();
}

QList<QPair<QString, QString> > DictionaryAbstract::results()
{
    return m_results;
}
