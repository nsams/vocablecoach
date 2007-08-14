//
// C++ Implementation: CommandAdd
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandAdd.h"
#include "VocableListModel.h"

CommandAdd::CommandAdd(VocableListModel* model, Vocable* vocable, int position, QUndoCommand * parent)
   : QUndoCommand(parent), m_vocableListModel(model), m_vocable(vocable)
{
    if (position == -1) {
        m_position = m_vocableListModel->rowCount();
    } else {
        m_position = position;
    }
    setText(QObject::tr("Add Vocable"));
}

void CommandAdd::redo()
{
    m_vocableListModel->insertVocable(m_position, m_vocable);
}

void CommandAdd::undo()
{
    m_vocableListModel->removeRow(m_position);
}

