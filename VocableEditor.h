//
// C++ Interface: VocableEditor
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOCABLEEDITOR_H
#define VOCABLEEDITOR_H

#include <QDialog>
#include "ui/ui_VocableEditor.h"

class QHttp;
class Vocable;
class VocableListModel;
class QTimer;
class QUndoStack;
class DictionaryAbstract;
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class VocableEditor : public QDialog, private Ui::VocableEdit
{
    Q_OBJECT
private:
    VocableEditor(VocableListModel* model, QUndoStack* undoStack, QWidget *parent = 0);
public:

    static int editVocable(VocableListModel* model, Vocable* vocable, QUndoStack* undoStack);
    static void addVocable(VocableListModel* model, QUndoStack* undoStack);

private:
    bool eventFilter(QObject *target, QEvent *event);
    QUndoStack* m_undoStack;
    VocableListModel* m_vocableListModel;
    DictionaryAbstract* m_dictionaryNative;
    DictionaryAbstract* m_dictionaryForeign;

protected:
    QTimer* startTranslationTimerNative;
    QTimer* startTranslationTimerForeign;
    QString _translateTextNative;
    QString _translateTextForeign;

private slots:
    void translationDone(bool error);
    void lookupWordNative();
    void lookupWordForeign();
    void nativeTextChanged();
    void foreignTextChanged();
    void addVocable();
};

#endif
