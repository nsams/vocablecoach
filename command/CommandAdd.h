//
// C++ Interface: CommandAdd
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDADD_H
#define COMMANDADD_H

#include <QUndoCommand>
class VocableListModel;
class Vocable;
/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class CommandAdd : public QUndoCommand
{
    public:
        CommandAdd(VocableListModel* model, Vocable* vocable, int position=-1, QUndoCommand * parent = 0);
        void redo();
        void undo();
    private:
        VocableListModel* m_vocableListModel;
        Vocable* m_vocable;
        int m_position;
};

#endif
