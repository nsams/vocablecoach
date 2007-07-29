//
// C++ Interface: VocableList
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOCABLELIST_H
#define VOCABLELIST_H
#include "ui/ui_VocableList.h"
#include <QWidget>
#include <QList>
#include <QDebug>
class VocableListModel;
class VocableListModelFilter;
class Vocable;

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class VocableList : public QWidget, private Ui::VocableList
{
	Q_OBJECT
public:
    VocableList(QWidget* parent = 0);
    ~VocableList();

    void addListAction(QAction* action);
    
    void setModel(VocableListModel* model);
    int columnWidth(int num);

    QItemSelection selection() const;
    Vocable* currentVocable();
    int currentRow();
    QList<int> selectedRows();
    QList<Vocable*> selectedVocables();

private slots:
    void boxFilterChanged(int box);
    void textFilterChanged(const QString& text);
    void refreshBoxFilter();

public slots:
    void selectAll();

signals:
    void doubleClicked(const QModelIndex& index);
    void selectionChanged(const QItemSelection&, const QItemSelection&);

// signals:
//     void editVocable(Vocable* vocable);

private:
    VocableListModel* m_vocableListModel;
    VocableListModelFilter* m_filteredVocableListModel;
};

#endif
