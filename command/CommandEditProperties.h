//
// C++ Interface: CommandEditProperties
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDEDITPROPERTIES_H
#define COMMANDEDITPROPERTIES_H

#include <QUndoCommand>
class VocableListModel;

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class CommandEditProperties : public QUndoCommand
{
public:
    CommandEditProperties(VocableListModel* model, QUndoCommand* parent = 0);
    void setTitle(QString title);
    void setAuthors(QString authors);
    void setForeignLanguage(QString foreignLanguage);
    void setNativeLanguage(QString nativeLanguage);

    void redo();
    void undo();

private:
    VocableListModel* m_vocableListModel;

    QString m_oldTitle;
    QString m_oldAuthors;
    QString m_oldForeignLanguage;
    QString m_oldNativeLanguage;

    QString m_newTitle;
    QString m_newAuthors;
    QString m_newForeignLanguage;
    QString m_newNativeLanguage;
};

#endif
