//
// C++ Interface: CommandEdit
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDEDIT_H
#define COMMANDEDIT_H

#include <QUndoCommand>
class Vocable;
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class CommandEdit : public QUndoCommand
{
    public:
        CommandEdit(Vocable* vocable, QUndoCommand * parent=0);
        void setNative(QString native);
        void setForeign(QString foreign);
        void setLesson(QString lesson);

        void redo();
        void undo();

    private:
        Vocable* m_vocable;

        QString m_oldNative;
        QString m_oldForeign;
        QString m_oldLesson;

        QString m_newNative;
        QString m_newForeign;
        QString m_newLesson;
};

#endif
