//
// C++ Implementation: CommandQuizAnswer
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandQuizAnswer.h"
#include <QDebug>

CommandQuizAnswer::CommandQuizAnswer(Vocable* vocable, Vocable::Direction direction, QUndoCommand* parent)
 : QUndoCommand(parent)
{
    m_data.direction = direction;
    m_data.vocable = vocable;
    m_data.oldBox = vocable->box(direction);
    m_data.oldLastQuery = vocable->lastQuery(direction);
    m_data.oldQueryCount = vocable->queryCount(direction);
    m_data.oldBadCount = vocable->badCount(direction);
    m_data.newBox = vocable->box(direction);
    m_data.newLastQuery = QDateTime::currentDateTime();
    m_data.newQueryCount = vocable->queryCount(direction);
    m_data.newBadCount = vocable->badCount(direction);


    setText(QObject::tr("Vocable Quiz"));
}

void CommandQuizAnswer::setBox(int box)
{
    m_data.newBox = box;
}
void CommandQuizAnswer::setBadCount(int badCount)
{
    m_data.newBadCount = badCount;
}
void CommandQuizAnswer::incrementBadCount()
{
    m_data.newBadCount++;
}
void CommandQuizAnswer::incrementQueryCount()
{
    m_data.newQueryCount++;
}
void CommandQuizAnswer::setQueryCount(int queryCount)
{
    m_data.newQueryCount = queryCount;
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
    m_data.vocable->setBox(m_data.direction, m_data.newBox);
    m_data.vocable->setLastQuery(m_data.direction, m_data.newLastQuery);
    m_data.vocable->setQueryCount(m_data.direction, m_data.newQueryCount);
    m_data.vocable->setBadCount(m_data.direction, m_data.newBadCount);
    foreach (CommandQuizAnswerData data, m_mergedDataList) {
        data.vocable->setBox(data.direction, data.newBox);
        data.vocable->setLastQuery(data.direction, data.newLastQuery);
        data.vocable->setQueryCount(data.direction, data.newQueryCount);
        data.vocable->setBadCount(data.direction, data.newBadCount);
    }
}

void CommandQuizAnswer::undo()
{
    m_data.vocable->setBox(m_data.direction, m_data.oldBox);
    m_data.vocable->setLastQuery(m_data.direction, m_data.oldLastQuery);
    m_data.vocable->setQueryCount(m_data.direction, m_data.oldQueryCount);
    m_data.vocable->setBadCount(m_data.direction, m_data.oldBadCount);
    foreach (CommandQuizAnswerData data, m_mergedDataList) {
        data.vocable->setBox(data.direction, data.oldBox);
        data.vocable->setLastQuery(data.direction, data.oldLastQuery);
        data.vocable->setQueryCount(data.direction, data.oldQueryCount);
        data.vocable->setBadCount(data.direction, data.oldBadCount);
    }
}
