//
// C++ Implementation: CommandResetBox
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
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
        m_oldBox[voc][Vocable::NativeToForeign] = voc->box(Vocable::NativeToForeign);
        m_oldBox[voc][Vocable::ForeignToNative] = voc->box(Vocable::ForeignToNative);
    }
    setText(QObject::tr("Reset Box"));
}


void CommandResetBox::redo()
{
    QHash<Vocable*, QHash<Vocable::Direction, int> >::iterator i = m_oldBox.begin();
    while (i != m_oldBox.end()) {
        i.key()->setBox(Vocable::NativeToForeign, 0);
        i.key()->setBox(Vocable::ForeignToNative, 0);
        ++i;
    }
}

void CommandResetBox::undo()
{
    QHash<Vocable*, QHash<Vocable::Direction, int> >::iterator i = m_oldBox.begin();
    while (i != m_oldBox.end()) {
        i.key()->setBox(Vocable::NativeToForeign, i.value()[Vocable::NativeToForeign]);
        i.key()->setBox(Vocable::ForeignToNative, i.value()[Vocable::ForeignToNative]);
        ++i;
    }
}



