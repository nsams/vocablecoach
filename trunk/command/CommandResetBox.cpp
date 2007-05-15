//
// C++ Implementation: CommandResetBox
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandResetBox.h"
#include "Vocable.h"
CommandResetBox::CommandResetBox(QList<Vocable*> vocables, QUndoCommand* parent)
 : QUndoCommand(parent)
{
    foreach (Vocable* voc, vocables) {
        m_oldBox[voc] = voc->box();
    }
    setText(QObject::tr("Reset Box"));
}


void CommandResetBox::redo()
{
    QHash<Vocable*, int>::const_iterator i = m_oldBox.constBegin();
    while (i != m_oldBox.constEnd()) {
        i.key()->setBox(0);
        ++i;
    }
}

void CommandResetBox::undo()
{
    QHash<Vocable*, int>::const_iterator i = m_oldBox.constBegin();
    while (i != m_oldBox.constEnd()) {
        i.key()->setBox(i.value());
        ++i;
    }
}



