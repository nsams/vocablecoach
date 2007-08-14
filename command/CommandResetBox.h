//
// C++ Interface: CommandResetBox
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDRESETBOX_H
#define COMMANDRESETBOX_H

#include <QUndoCommand>
#include <QHash>
#include <QList>
#include "Vocable.h"

/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class CommandResetBox : public QUndoCommand
{
public:
    CommandResetBox(QList<Vocable*> vocables, QUndoCommand* parent=0);
    void redo();
    void undo();

private:
    QHash<Vocable*, QHash<Vocable::Direction, int> > m_oldBox;
};

#endif
