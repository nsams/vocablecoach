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
#include "VocableListModel.h"

CommandModifyLesson::CommandModifyLesson(VocableListModel* model, QList<int> selectedRows, const QString& newLesson, QUndoCommand* parent)
 : QUndoCommand(parent), m_vocableListModel(model), m_rows(selectedRows), m_newLesson(newLesson)
{
    foreach(int row, m_rows) {
        Vocable* vocable = m_vocableListModel->vocable(row);
        m_oldLesson << vocable->lesson();
    }
}

void CommandModifyLesson::redo()
{
    foreach(int row, m_rows) {
        Vocable* vocable = m_vocableListModel->vocable(row);
        vocable->setLesson(m_newLesson);
    }
}

void CommandModifyLesson::undo()
{
    for (int i = 0; i < m_rows.size(); i++) {
        Vocable* vocable = m_vocableListModel->vocable(m_rows.at(i));
        vocable->setLesson(m_oldLesson.at(i));
    }
}
