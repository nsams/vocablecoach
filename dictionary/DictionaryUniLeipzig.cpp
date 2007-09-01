//
// C++ Implementation: DictionaryUniLeipzig
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryUniLeipzig.h"
#include <QUrl>
#include <QHttp>
#include <QDebug>
DictionaryUniLeipzig::DictionaryUniLeipzig(const QMap<QString, QVariant>& settings, QObject *parent)
    : DictionaryHttpAbstract(settings, parent)
{
}

QString DictionaryUniLeipzig::host()
{
    return QString("dict.uni-leipzig.de");
}
void DictionaryUniLeipzig::lookupWord(const QString& word)
{
    DictionaryHttpAbstract::lookupWord(word);
    QUrl url("/index.php");
    url.addQueryItem("wort", word.simplified());
    url.addQueryItem("anzahlen", "on");
    url.addQueryItem("fuzzy", "on");
    m_http->get(url.path()+"?"+url.encodedQuery());
}

void DictionaryUniLeipzig::processData(bool error)
{
    if (!error) {
        QString Text(m_http->readAll());
        QRegExp rx("<table width=\"100%\" bgcolor=\"#eeeeee\" cellpadding=\"2\" border=\"0\">(.*)</table>");
        rx.setMinimal(true);
        rx.indexIn(Text);
        Text = rx.cap(1);
        rx = QRegExp("<tr><td bgcolor=\"#ffffff\">(.*)</td><td align=\"right\" bgcolor=\"#ffffff\">(.*)</td>.*</tr>");
        rx.setMinimal(true);

        int pos = 0;
        while ((pos = rx.indexIn(Text, pos)) != -1) {
            pos += rx.matchedLength();
            QPair<QString, QString> row;
            row.first = rx.cap(1);
            row.first.replace(QRegExp("<a [^>]*>([^<]+)</a> \\([0-9]+\\)", Qt::CaseInsensitive), "\\1");
            row.first = _cleanHtml(row.first);
            row.second = _cleanHtml(rx.cap(2));
            m_results << row;
        }
    }
    emit done(error);
}

QPair<QString, QString> DictionaryUniLeipzig::headerText()
{
    return QPair<QString, QString>(tr("German"), tr("English"));
}
