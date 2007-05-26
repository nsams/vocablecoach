//
// C++ Interface: CommandQuizAnswer
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDQUIZANSWER_H
#define COMMANDQUIZANSWER_H

#include <QUndoCommand>
#include <QDateTime>
#include <QList>
class Vocable;

struct CommandQuizAnswerData {
    Vocable* vocable;
    QDateTime oldLastQuery;
    int oldBox;
    int oldBadCount;
    int oldQueryCount;
    QDateTime newLastQuery;
    int newBox;
    int newBadCount;
    int newQueryCount;
};
/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class CommandQuizAnswer : public QUndoCommand
{
public:
    CommandQuizAnswer(Vocable* vocable, QUndoCommand* parent = 0);
    void setBox(int box);
    void setQueryCount(int queryCount);
    void setBadCount(int badCount);
    void incrementBadCount();
    void incrementQueryCount();
    
    int id() const;
    bool mergeWith(const QUndoCommand *other);

    void redo();
    void undo();
private:
    CommandQuizAnswerData m_data;
    QList<CommandQuizAnswerData> m_mergedDataList;
};

#endif
