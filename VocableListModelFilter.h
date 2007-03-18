//
// C++ Interface: VocableListModelFilter
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOCABLELISTMODELFILTER_H
#define VOCABLELISTMODELFILTER_H

#include <QSortFilterProxyModel>

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class VocableListModelFilter : public QSortFilterProxyModel
{
Q_OBJECT
public:
    VocableListModelFilter(QObject *parent = 0);
    ~VocableListModelFilter();
    void setBoxFilter(int filter);
    void setTextFilter(const QString &filter);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    int m_boxFilter;
    QString m_textFilter;

};

#endif
