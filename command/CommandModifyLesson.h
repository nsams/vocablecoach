//
// C++ Interface: CommandModifyLesson
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDMODIFYLESSON_H
#define COMMANDMODIFYLESSON_H

#include <QUndoCommand>
#include <QList>
#include <QString>
class VocableListModel;
/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class CommandModifyLesson : public QUndoCommand
{
public:
    CommandModifyLesson(VocableListModel* m_vocableListModel, QList<int> selectedRows, const QString& newLesson, QUndoCommand* parent=0);
    void redo();
    void undo();
private:
    VocableListModel* m_vocableListModel;
    QList<int> m_rows;
    QList<QString> m_oldLesson;
    QString m_newLesson;
};

#endif
