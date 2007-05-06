//
// C++ Implementation: CommandEdit
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandEdit.h"
#include "Vocable.h"
CommandEdit::CommandEdit(Vocable* vocable, QUndoCommand * parent)
    : QUndoCommand(parent), m_vocable(vocable)
{
    m_oldNative = m_vocable->native();
    m_oldForeign = m_vocable->foreign();
    m_oldLesson = m_vocable->lesson();
    
    setText(QObject::tr("Edit Vocable"));
}
void CommandEdit::setNative(QString native)
{
    m_newNative = native;
}
void CommandEdit::setForeign(QString foreign)
{
    m_newForeign = foreign;
}
void CommandEdit::setLesson(QString lesson)
{
    m_newLesson = lesson;
}

void CommandEdit::redo()
{
    m_vocable->setNative(m_newNative);
    m_vocable->setForeign(m_newForeign);
    m_vocable->setLesson(m_newLesson);
}

void CommandEdit::undo()
{
    m_vocable->setNative(m_oldNative);
    m_vocable->setForeign(m_oldForeign);
    m_vocable->setLesson(m_oldLesson);
}
