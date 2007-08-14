//
// C++ Interface: CommandModifyLesson
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDMODIFYLESSON_H
#define COMMANDMODIFYLESSON_H

#include <QUndoCommand>
#include <QList>
#include <QString>
#include <QHash>
class Vocable;
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class CommandModifyLesson : public QUndoCommand
{
public:
    CommandModifyLesson(QList<Vocable*> vocable, const QString& newLesson, QUndoCommand* parent=0);
    void redo();
    void undo();
private:
    QHash<Vocable*, QString> m_oldLesson;
    QString m_newLesson;
};

#endif
