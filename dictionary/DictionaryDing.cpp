//
// C++ Implementation: DictionaryDing
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//         Michael Rex <me@rexi.org>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryDing.h"
#include "dictionary/DictionaryDingLookupThread.h"
#include <QDebug>

DictionaryDing::DictionaryDing(const QMap<QString, QVariant>& settings, QObject *parent)
    : DictionaryAbstract(settings, parent), m_thread(0)
{
}

void DictionaryDing::lookupWord(const QString& word)
{
    DictionaryAbstract::lookupWord(word);
    if (m_thread) {
        //lookup is allready running
        return;
    }
    m_thread = new DictionaryDingLookupThread(word);
    m_thread->start();
    connect(m_thread, SIGNAL(finished()), this, SLOT(processData()));
}

void DictionaryDing::processData()
{
    m_results = m_thread->results();
    emit done(false);
    delete m_thread;
    m_thread = 0;
}

QPair<QString, QString> DictionaryDing::headerText()
{
    return QPair<QString, QString>(tr("Language 1"), tr("Language 2"));
}

