//
// C++ Implementation: CommandQuizAnswer
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandQuizAnswer.h"
#include "Vocable.h"

CommandQuizAnswer::CommandQuizAnswer(Vocable* vocable, QUndoCommand* parent)
 : QUndoCommand(parent)
{
    m_data.vocable = vocable;
    m_data.oldBox = vocable->box();
    m_data.oldLastQuery = vocable->lastQuery();
    m_data.newLastQuery = QDateTime::currentDateTime();


    setText(QObject::tr("Vocable Quiz"));
}

void CommandQuizAnswer::setBox(int box)
{
    m_data.newBox = box;
}
int CommandQuizAnswer::id() const
{
    return 1;
}
bool CommandQuizAnswer::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) // make sure other is also an AppendText command
        return false;
    m_mergedDataList << static_cast<const CommandQuizAnswer*>(other)->m_data;
    return true;
}


void CommandQuizAnswer::redo()
{
    m_data.vocable->setBox(m_data.newBox);
    m_data.vocable->setLastQuery(m_data.newLastQuery);
    foreach (CommandQuizAnswerData data, m_mergedDataList) {
        data.vocable->setBox(data.newBox);
        data.vocable->setLastQuery(data.newLastQuery);
    }
}

void CommandQuizAnswer::undo()
{
    m_data.vocable->setBox(m_data.oldBox);
    m_data.vocable->setLastQuery(m_data.oldLastQuery);
    foreach (CommandQuizAnswerData data, m_mergedDataList) {
        data.vocable->setBox(data.oldBox);
        data.vocable->setLastQuery(data.oldLastQuery);
    }
}
