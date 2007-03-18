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
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class VocableEditor : public QDialog, private Ui::VocableEdit
{
    Q_OBJECT
private:
    VocableEditor(QWidget *parent = 0);
public:
    ~VocableEditor();

    static VocableEditor* getEditor();
    static int editVocable(VocableListModel* model, Vocable* vocable);
    static void addVocable(VocableListModel* model);

private:
	static VocableEditor* m_editor;
    bool eventFilter(QObject *target, QEvent *event);

protected:
    QHttp* httpLeo;
    QTimer* startTranslationTimer;
    QString _translateText;

private slots:
    void readLeoData(bool);
    void translateLeo();
    void nativeTextChanged();
    void foreignTextChanged();
};

#endif
