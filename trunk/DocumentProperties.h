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
#ifndef DOCUMENTPROPERTIES_H
#define DOCUMENTPROPERTIES_H

#include <QDialog>
#include "ui/ui_DocumentProperties.h"
class VocableListModel;
class QUndoStack;

/**
	@author Niko Sams <niko.sams@gmail.com>
 */
class DocumentProperties : public QDialog, private Ui::DocumentProperties
{
    Q_OBJECT
    public:
        DocumentProperties(VocableListModel* model, QUndoStack* undoStack, QWidget *parent = 0);
        ~DocumentProperties();
    private:
        VocableListModel* m_model;
        QUndoStack* m_undoStack;
    protected slots:
        void ok();
};

#endif
