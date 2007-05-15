//
// C++ Implementation: CommandModifyLesson
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandModifyLesson.h"
#include "Vocable.h"

CommandModifyLesson::CommandModifyLesson(QList<Vocable*> vocables, const QString& newLesson, QUndoCommand* parent)
 : QUndoCommand(parent), m_newLesson(newLesson)
{
    foreach (Vocable* voc, vocables) {
        m_oldLesson[voc] = voc->lesson();
    }
    setText(QObject::tr("Modify Lesson"));
}

void CommandModifyLesson::redo()
{
    QHash<Vocable*, QString>::const_iterator i = m_oldLesson.constBegin();
    while (i != m_oldLesson.constEnd()) {
        i.key()->setLesson(m_newLesson);
    }
}

void CommandModifyLesson::undo()
{
    QHash<Vocable*, QString>::const_iterator i = m_oldLesson.constBegin();
    while (i != m_oldLesson.constEnd()) {
        i.key()->setLesson(i.value());
    }
}
