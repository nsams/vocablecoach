//
// C++ Implementation: DictionaryDictCc
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryDictCc.h"
#include <QUrl>
#include <QHttp>
#include <QDebug>
DictionaryDictCc::DictionaryDictCc(QObject *parent)
 : DictionaryHttpAbstract(parent)
{
}

QString DictionaryDictCc::host()
{
    return QString("www.dict.cc");
}
void DictionaryDictCc::lookupWord(const QString& word)
{
    DictionaryHttpAbstract::lookupWord(word);
    QUrl url("/");
    url.addQueryItem("s", word.simplified());
    m_http->get(url.path()+"?"+url.encodedQuery());
}

void DictionaryDictCc::processData(bool error)
{
    if (!error) {
        QString Text(m_http->readAll());
        QRegExp rx("<table.*width=\"620\">(.*)</table>");
        rx.setMinimal(true);
        rx.indexIn(Text);
        Text = rx.cap(1);
        rx = QRegExp("<tr.*>.*<td class=td7nl>(.*)</td><td class=td7nl>(.*)</td>.*</tr>");
        rx.setMinimal(true);

        int pos = 0;
        while ((pos = rx.indexIn(Text, pos)) != -1) {
            pos += rx.matchedLength();
            QPair<QString, QString> row;
            row.first = _cleanHtml(rx.cap(1));
            row.second = _cleanHtml(rx.cap(2));
            m_results << row;
        }
    }
    emit done(error);
}

