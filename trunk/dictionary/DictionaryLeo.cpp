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
#include <QUrl>
#include <QHttp>
#include <QDebug>
#include "dictionary/DictionaryLeo.h"
#include "ui/ui_SettingsLeo.h"

DictionaryLeo::DictionaryLeo(const QMap<QString, QVariant>& settings, QObject *parent)
    : DictionaryHttpAbstract(settings, parent)
{
    if (m_settings["language"] == "") m_settings["language"] = "ende";
}
void DictionaryLeo::lookupWord(const QString& word)
{
    DictionaryHttpAbstract::lookupWord(word);
    QUrl url("/");
    url.addQueryItem("search", word.simplified());
    url.addQueryItem("lp", m_settings["language"].toString());
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

QMap<QString, QVariant> DictionaryLeo::editSettings()
{
    QDialog dialog;
    Ui::DictionarySettingsLeo ui;
    ui.setupUi(&dialog);

    QStringList languages;
    ui.languageComboBox->addItem(tr("English-German"), "ende");
    ui.languageComboBox->addItem(tr("French-German"), "frde");
    ui.languageComboBox->addItem(tr("Spanish-German"), "esde");
    ui.languageComboBox->setCurrentIndex(ui.languageComboBox->findData(m_settings["language"]));

    if (dialog.exec() == QDialog::Accepted) {
        m_settings["language"] = ui.languageComboBox->itemData(ui.languageComboBox->currentIndex()).toString();
    }
    return m_settings;
}

QPair<QString, QString> DictionaryLeo::headerText()
{
    QPair<QString, QString> ret;
    if (m_settings["language"] == "ende") {
        ret.first = tr("English");
    } else if (m_settings["language"] == "frde") {
        ret.first = tr("French");
    } else if (m_settings["language"] == "esde") {
        ret.first = tr("Spanish");
    }
    ret.second = tr("German");
    return ret;
}
