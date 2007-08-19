/*
 * resultitem.h
 *
 * Copyright (c) 2006 Michael Rex <me@rexi.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef DICTIONARYDINGRESULTITEM_H
#define DICTIONARYDINGRESULTITEM_H

#include <QString>
#include <QList>

/**
 * @author Michael Rex
 */
class DictionaryDingResultItem {

    public:
        DictionaryDingResultItem(const QString& text = 0, const int priority = 0);
        ~DictionaryDingResultItem();
    
        QString text() const;
        void setText(const QString& text);
    
        int priority() const;
        void setPriority(const int priority);
        void addToPriority(const int priority);
    
        bool operator<(const DictionaryDingResultItem& other) const;
    
    private:
        QString str;
        int prio;
};

#endif
