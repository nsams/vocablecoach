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
#include <QFileDialog>

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
    m_thread = new DictionaryDingLookupThread(m_settings["file"].toString(), word);
    m_thread->start();
    connect(m_thread, SIGNAL(finished()), this, SLOT(processData()));
}

void DictionaryDing::settingsOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(m_settingsDialog,
            tr("Open Dictionary"), QString(), tr("Dictionary Files (*.txt)"));
    if (!fileName.isEmpty()) {
        m_settingsUi->fileNameLineEdit->setText(fileName);
    }
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
    return QPair<QString, QString>("", "");
}

QMap<QString, QVariant> DictionaryDing::editSettings(QWidget* parent)
{
    m_settingsDialog = new QDialog(parent);
    m_settingsUi = new Ui::DictionarySettingsDing;
    m_settingsUi->setupUi(m_settingsDialog);

    m_settingsUi->fileNameLineEdit->setText(m_settings["file"].toString());
    connect(m_settingsUi->openFileButton, SIGNAL(clicked()), this, SLOT(settingsOpenFile()));


    if (m_settingsDialog->exec() == QDialog::Accepted) {
        m_settings["file"] = m_settingsUi->fileNameLineEdit->text();
    }
    delete m_settingsDialog;
    delete m_settingsUi;
    return m_settings;
}
