//
// C++ Implementation: VocableListModelFilter
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "VocableListModelFilter.h"

VocableListModelFilter::VocableListModelFilter(QObject *parent)
 : QSortFilterProxyModel(parent)
{
}


VocableListModelFilter::~VocableListModelFilter()
{
}




bool VocableListModelFilter::filterAcceptsRow(int sourceRow,
                                               const QModelIndex &sourceParent) const
{

    QModelIndex indexForeign = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex indexNative = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex indexBox = sourceModel()->index(sourceRow, 2, sourceParent);
    QModelIndex indexLesson = sourceModel()->index(sourceRow, 3, sourceParent);

    return (sourceModel()->data(indexForeign).toString().contains(m_textFilter)
            || sourceModel()->data(indexNative).toString().contains(m_textFilter)
            || sourceModel()->data(indexLesson).toString().contains(m_textFilter))
            && (m_boxFilter==-1 || sourceModel()->data(indexBox).toInt() == m_boxFilter);
}

void VocableListModelFilter::setBoxFilter(int filter)
{
    m_boxFilter = filter;
    filterChanged();
}

void VocableListModelFilter::setTextFilter(const QString &filter)
{
    m_textFilter = filter;
    filterChanged();
}
