//
// C++ Interface: StatisticsWidget
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include "ui/ui_StatisticsWidget.h"
#include <QList>
#include <QPair>
class VocableListModel;
class QLabel;

/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class StatisticsWidget : public QWidget, private Ui::StatisticsWidget
{
Q_OBJECT
public:
    StatisticsWidget(QWidget *parent = 0);
    ~StatisticsWidget();
    void setModel(VocableListModel* model);
public slots:
    void refresh();
private:
    VocableListModel* m_vocableListModel;
    QList< QPair<QLabel*, QLabel*> > m_boxLabels;
};

#endif
