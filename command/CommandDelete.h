//
// C++ Interface: CommandDelete
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDDELETE_H
#define COMMANDDELETE_H

#include <QUndoCommand>
#include <QList>
class VocableListModel;
class Vocable;
class QItemSelection;

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class CommandDelete : public QUndoCommand
{
public:
    CommandDelete(VocableListModel* model, QList<int> rows, QUndoCommand * parent = 0);
    void redo();
    void undo();
private:
    VocableListModel* m_vocableListModel;
    QList<int> m_rows;
    QList<Vocable*> m_vocables;
};

#endif
