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
#include "Vocable.h"
#include "VocableListModel.h"

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
    VocableListModel* model = qobject_cast<VocableListModel*>(sourceModel());
    if (!model) {
        return false;
    }
    return (model->vocable(sourceRow)->foreign().contains(m_textFilter)
            || model->vocable(sourceRow)->native().contains(m_textFilter)
            || model->vocable(sourceRow)->lesson().contains(m_textFilter))
            && (m_boxFilter==-1
            || model->vocable(sourceRow)->box(Vocable::NativeToForeign) == m_boxFilter
            || model->vocable(sourceRow)->box(Vocable::ForeignToNative) == m_boxFilter);
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
