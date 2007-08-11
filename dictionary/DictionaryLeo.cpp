//
// C++ Implementation: DictionaryLeo
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryLeo.h"
#include <QUrl>
#include <QHttp>
#include <QDebug>

DictionaryLeo::DictionaryLeo(QObject *parent)
    : DictionaryHttpAbstract(parent)
{
}
void DictionaryLeo::lookupWord(const QString& word)
{
    DictionaryHttpAbstract::lookupWord(word);
    QUrl url("/");
    url.addQueryItem("search", word.simplified());
    m_http->get(url.path()+"?"+url.encodedQuery());
}

QString DictionaryLeo::host()
{
    return QString("dict.leo.org");
}

void DictionaryLeo::processData(bool error)
{
    if (!error) {
        QString Text(m_http->readAll());
        QRegExp rx("<table.*id=\"results\".*>(.*)</table>");
        rx.setMinimal(true);
        rx.indexIn(Text);
        Text = rx.cap(1);

        rx = QRegExp("<tr.*>(.*)</tr>");
        rx.setMinimal(true);

        int pos = 0;
        while ((pos = rx.indexIn(Text, pos)) != -1) {
            pos += rx.matchedLength();
            QString textRow = rx.cap(1);
            QRegExp rxRow("<TD.*COLSPAN=5.*>.*</TD>");
            if (rxRow.indexIn(textRow) != -1) {
                //headline, ignore for now (eg "Unmittelbare Treffer")
                continue;
            }
            rxRow = QRegExp("<td.*>.*</td>.*<td.*>(.*)</td>.*<td.*>.*</td>.*<td.*>(.*)</td>.*<td.*>.*</td>");
            if (rxRow.indexIn(textRow) != -1) {
                //normal result-row
                QPair<QString, QString> row;
                row.first = _cleanHtml(rxRow.cap(1));
                row.second = _cleanHtml(rxRow.cap(2));
                m_results << row;
                continue;
            }
            //else: unknown row
        }
    }
    emit done(error);
}
