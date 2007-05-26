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
#include <QDebug>

CommandQuizAnswer::CommandQuizAnswer(Vocable* vocable, QUndoCommand* parent)
 : QUndoCommand(parent)
{
    m_data.vocable = vocable;
    m_data.oldBox = vocable->box();
    m_data.oldLastQuery = vocable->lastQuery();
    m_data.oldQueryCount = vocable->queryCount();
    m_data.oldBadCount = vocable->badCount();
    m_data.newBox = vocable->box();
    m_data.newLastQuery = QDateTime::currentDateTime();
    m_data.newQueryCount = vocable->queryCount();
    m_data.newBadCount = vocable->badCount();


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
    m_data.vocable->setBox(m_data.newBox);
    m_data.vocable->setLastQuery(m_data.newLastQuery);
    m_data.vocable->setQueryCount(m_data.newQueryCount);
    m_data.vocable->setBadCount(m_data.newBadCount);
    foreach (CommandQuizAnswerData data, m_mergedDataList) {
        data.vocable->setBox(data.newBox);
        data.vocable->setLastQuery(data.newLastQuery);
        data.vocable->setQueryCount(data.newQueryCount);
        data.vocable->setBadCount(data.newBadCount);
    }
}

void CommandQuizAnswer::undo()
{
    m_data.vocable->setBox(m_data.oldBox);
    m_data.vocable->setLastQuery(m_data.oldLastQuery);
    m_data.vocable->setQueryCount(m_data.oldQueryCount);
    m_data.vocable->setBadCount(m_data.oldBadCount);
    foreach (CommandQuizAnswerData data, m_mergedDataList) {
        data.vocable->setBox(data.oldBox);
        data.vocable->setLastQuery(data.oldLastQuery);
        data.vocable->setQueryCount(data.oldQueryCount);
        data.vocable->setBadCount(data.oldBadCount);
    }
}
