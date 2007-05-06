//
// C++ Implementation: CommandEditProperties
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandEditProperties.h"
#include "VocableListModel.h"

CommandEditProperties::CommandEditProperties(VocableListModel* model, QUndoCommand* parent)
 : QUndoCommand(parent), m_vocableListModel(model)
{
    m_oldTitle = m_vocableListModel->title();
    m_oldAuthors = m_vocableListModel->authors();
    m_oldForeignLanguage = m_vocableListModel->foreignLanguage();
    m_oldNativeLanguage = m_vocableListModel->nativeLanguage();
    
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


void CommandEditProperties::redo()
{
    m_vocableListModel->setTitle(m_newTitle);
    m_vocableListModel->setAuthors(m_newAuthors);
    m_vocableListModel->setForeignLanguage(m_newForeignLanguage);
    m_vocableListModel->setNativeLanguage(m_newNativeLanguage);
}

void CommandEditProperties::undo()
{
    m_vocableListModel->setTitle(m_oldTitle);
    m_vocableListModel->setAuthors(m_oldAuthors);
    m_vocableListModel->setForeignLanguage(m_oldForeignLanguage);
    m_vocableListModel->setNativeLanguage(m_oldNativeLanguage);
}

