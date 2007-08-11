//
// C++ Implementation: DictionaryWorterbuchInfo
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryWorterbuchInfo.h"
#include <QUrl>
#include <QHttp>
#include <QDebug>
DictionaryWorterbuchInfo::DictionaryWorterbuchInfo(QObject *parent)
 : DictionaryHttpAbstract(parent)
{
}

QString DictionaryWorterbuchInfo::host()
{
    return QString("www.woerterbuch.info");
}
void DictionaryWorterbuchInfo::lookupWord(const QString& word)
{
    DictionaryHttpAbstract::lookupWord(word);
    QUrl url("/");
    url.addQueryItem("query", word.simplified());
    url.addQueryItem("s", "dict");
    m_http->get(url.path()+"?"+url.encodedQuery());
}

void DictionaryWorterbuchInfo::processData(bool error)
{
    if (!error) {
        QString Text(m_http->readAll());
        QRegExp rx("<td class=\"standard\"><table.*width=\"600\">(.*)</table>");
        rx.setMinimal(true);
        rx.indexIn(Text);
        Text = rx.cap(1);
        rx = QRegExp("<tr.*>.*<td.*class=\"hl\" width=\"50%\">(.*)</td><td.*class=\"hl\" width=\"50%\">(.*)</td>.*</tr>");
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
