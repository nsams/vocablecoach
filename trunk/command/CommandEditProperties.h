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
#include <QMap>
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
    void setForeignLanguageDict(QString dict);
    void setNativeLanguageDict(QString dict);
    void setNativeLanguageSettings(QMap<QString, QVariant> settings);
    void setForeignLanguageSettings(QMap<QString, QVariant> settings);
    QMap<QString, QVariant> getNativeLanguageSettings() { return m_newNativeLanguageSettings; }
    QMap<QString, QVariant> getForeignLanguageSettings() { return m_oldNativeLanguageSettings; }

    void redo();
    void undo();

private:
    VocableListModel* m_vocableListModel;

    QString m_oldTitle;
    QString m_oldAuthors;
    QString m_oldForeignLanguage;
    QString m_oldNativeLanguage;
    QString m_oldForeignLanguageDict;
    QString m_oldNativeLanguageDict;
    QMap<QString, QVariant> m_oldForeignLanguageSettings;
    QMap<QString, QVariant> m_oldNativeLanguageSettings;

    QString m_newTitle;
    QString m_newAuthors;
    QString m_newForeignLanguage;
    QString m_newNativeLanguage;
    QString m_newForeignLanguageDict;
    QString m_newNativeLanguageDict;
    QMap<QString, QVariant> m_newForeignLanguageSettings;
    QMap<QString, QVariant> m_newNativeLanguageSettings;
};

#endif
