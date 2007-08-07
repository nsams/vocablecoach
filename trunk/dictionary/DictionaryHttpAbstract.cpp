//
// C++ Implementation: DictionaryHttpAbstract
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryHttpAbstract.h"
#include <QHttp>

DictionaryHttpAbstract::DictionaryHttpAbstract(QObject *parent)
 : DictionaryAbstract(parent)
{
    m_http = new QHttp(this);
    connect(m_http, SIGNAL(done(bool)), this, SLOT(processData(bool)));
}

    
void DictionaryHttpAbstract::lookupWord(const QString& word)
{
    Q_UNUSED(word);
    DictionaryAbstract::lookupWord(word);
    m_http->setHost(host());
}
