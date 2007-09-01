//
// C++ Implementation: CommandEditProperties
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandEditProperties.h"
#include "VocableListModel.h"
#include <QDebug>

CommandEditProperties::CommandEditProperties(VocableListModel* model, QUndoCommand* parent)
 : QUndoCommand(parent), m_vocableListModel(model)
{
    m_oldTitle = m_vocableListModel->title();
    m_oldAuthors = m_vocableListModel->authors();
    m_oldForeignLanguage = m_vocableListModel->foreignLanguage();
    m_oldNativeLanguage = m_vocableListModel->nativeLanguage();
    m_oldForeignLanguageDict = m_vocableListModel->foreignLanguageDict();
    m_oldNativeLanguageDict = m_vocableListModel->nativeLanguageDict();
    m_oldForeignLanguageSettings = m_vocableListModel->nativeLanguageSettings();
    m_oldNativeLanguageSettings = m_vocableListModel->nativeLanguageSettings();
    
    setText(QObject::tr("Modify Document Properties"));
}

void CommandEditProperties::setTitle(QString title)
{
    m_newTitle = title;
}
void CommandEditProperties::setAuthors(QString authors)
{
    m_newAuthors = authors;
}
void CommandEditProperties::setForeignLanguage(QString foreignLanguage)
{
    m_newForeignLanguage = foreignLanguage;
}
void CommandEditProperties::setNativeLanguage(QString nativeLanguage)
{
    m_newNativeLanguage = nativeLanguage;
}

void CommandEditProperties::setForeignLanguageDict(QString dict)
{
    m_newForeignLanguageDict = dict;
}
void CommandEditProperties::setNativeLanguageDict(QString dict)
{
    m_newNativeLanguageDict = dict;
}
void CommandEditProperties::setNativeLanguageSettings(QMap<QString, QVariant> settings)
{
    m_newNativeLanguageSettings = settings;
}
void CommandEditProperties::setForeignLanguageSettings(QMap<QString, QVariant> settings)
{
    m_newForeignLanguageSettings = settings;
}
void CommandEditProperties::redo()
{
    m_vocableListModel->setTitle(m_newTitle);
    m_vocableListModel->setAuthors(m_newAuthors);
    m_vocableListModel->setForeignLanguage(m_newForeignLanguage);
    m_vocableListModel->setNativeLanguage(m_newNativeLanguage);
    m_vocableListModel->setForeignLanguageDict(m_newForeignLanguageDict);
    m_vocableListModel->setNativeLanguageDict(m_newNativeLanguageDict);
    m_vocableListModel->setNativeLanguageSettings(m_newNativeLanguageSettings);
    m_vocableListModel->setForeignLanguageSettings(m_newForeignLanguageSettings);
}

void CommandEditProperties::undo()
{
    m_vocableListModel->setTitle(m_oldTitle);
    m_vocableListModel->setAuthors(m_oldAuthors);
    m_vocableListModel->setForeignLanguage(m_oldForeignLanguage);
    m_vocableListModel->setNativeLanguage(m_oldNativeLanguage);
    m_vocableListModel->setForeignLanguageDict(m_oldForeignLanguageDict);
    m_vocableListModel->setNativeLanguageDict(m_oldNativeLanguageDict);
    m_vocableListModel->setNativeLanguageSettings(m_oldNativeLanguageSettings);
    m_vocableListModel->setForeignLanguageSettings(m_oldForeignLanguageSettings);
}

