//
// C++ Implementation: CommandModifyLesson
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandModifyLesson.h"
#include "Vocable.h"
#include <QDebug>

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
    QHash<Vocable*, QString>::iterator i = m_oldLesson.begin();
    while (i != m_oldLesson.end()) {
        i.key()->setLesson(m_newLesson);
        ++i;
    }
}

void CommandModifyLesson::undo()
{
    QHash<Vocable*, QString>::iterator i = m_oldLesson.begin();
    while (i != m_oldLesson.end()) {
        i.key()->setLesson(i.value());
        ++i;
    }
}
