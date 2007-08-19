//
// C++ Implementation: DictionaryDingLookupThread
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryDingLookupThread.h"
#include "dictionary/DictionaryDingResultItem.h"
#include <QFile>
#include <QList>
#include <QRegExp>
#include <QStringList>

DictionaryDingLookupThread::DictionaryDingLookupThread(const QString& word, QObject *parent)
 : QThread(parent), m_word(word)
{
}


DictionaryDingLookupThread::~DictionaryDingLookupThread()
{
}

void DictionaryDingLookupThread::run()
{
    QFile file("de-en.txt");
    if (!file.exists()) {
//        QMessageBox::warning(0, tr("VocableCoach"), tr("Can't find dictionary-file de-en.txt."));
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QList<DictionaryDingResultItem> reslist;
    while (!file.atEnd()) {
        QString line = QString::fromUtf8(file.readLine());
        if (line.contains(m_word, Qt::CaseInsensitive)) {
            DictionaryDingResultItem item(line.trimmed());
            reslist.append(item);
        }
    }
    const int VALUE_ANFANG = 100;
    const int VALUE_ERSTEZEILE = 100;
    const int VALUE_EINZELWORT = 80;
    const int VALUE_ABK = 10;
    
    QRegExp ml("\\|");
    QRegExp abkuerzung(" : ");
    QRegExp anfang_ger("^" + m_word + "(;| ::)", Qt::CaseInsensitive);
    QRegExp anfang_eng(":: (to )?" + m_word + "(;|$)", Qt::CaseInsensitive);
    QRegExp anfang_ger_ml("^" + m_word + "(;| ).*\\|.* ::", Qt::CaseInsensitive);
    QRegExp anfang_eng_ml(":: (to )?" + m_word + "(;.*| )\\|", Qt::CaseInsensitive);
    QRegExp einzelwort("; " + m_word + "(;|$)", Qt::CaseInsensitive);
    QRegExp erstezeile_ger("^[^\\|]*" + m_word + ".*\\|.* ::", Qt::CaseInsensitive);
    QRegExp erstezeile_eng(":: [^\\|]*" + m_word + ".*\\|", Qt::CaseInsensitive);
    
    QRegExp brackets(" \\([^.\\)]*\\)");
    QRegExp curlybrackets(" \\{[^.\\}]*\\}");
    QRegExp squarebrackets(" \\[[^\\]]*\\]");
    
    QList<DictionaryDingResultItem>::iterator it;
    for (it = reslist.begin(); it != reslist.end(); ++it) {
        QString text = (*it).text();
        text.replace(brackets, "");
        text.replace(curlybrackets, "");
        text.replace(squarebrackets, "");

        if(ml.indexIn(text) != -1) { // multiline result
            if((anfang_ger_ml.indexIn(text) != -1) || (anfang_eng_ml.indexIn(text) != -1)) {
                (*it).addToPriority(VALUE_ANFANG);
            } else if((erstezeile_ger.indexIn(text) != -1) || (erstezeile_eng.indexIn(text) != -1)) {
                (*it).addToPriority(VALUE_ERSTEZEILE);
            }
        } else { // singleline result
            if((anfang_ger.indexIn(text) != -1) || (anfang_eng.indexIn(text) != -1)) {
                (*it).addToPriority(VALUE_ANFANG);
            } else if(einzelwort.indexIn(text) != -1) {
                (*it).addToPriority(VALUE_EINZELWORT);
            }

            if(abkuerzung.indexIn(text) != -1) {
                (*it).addToPriority(VALUE_ABK);
            }
        }
    }
    qSort(reslist);

    for (it = reslist.begin(); it != reslist.end(); ++it) {
        QRegExp rx("^(.*) :: (.*)$");
        if (rx.indexIn((*it).text()) == -1) continue;
        QPair<QString, QString> r;
        r.first = rx.cap(2).replace(" | ", "\n  ");
        r.second = rx.cap(1).replace(" | ", "\n  ");
        m_results << r;
    }
}

QList<QPair<QString, QString> > DictionaryDingLookupThread::results()
{
    return m_results;
}
