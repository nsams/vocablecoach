/*
 * resultitem.cpp
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

#include "dictionary/DictionaryDingResultItem.h"

DictionaryDingResultItem::DictionaryDingResultItem(const QString& text, const int priority) : str(text), prio(priority) {
    
}

DictionaryDingResultItem::~DictionaryDingResultItem() {
    
}

QString DictionaryDingResultItem::text() const {
    return str;
}

void DictionaryDingResultItem::setText(const QString& text) {
    this->str = text;
}

int DictionaryDingResultItem::priority() const {
    return prio;
}

void DictionaryDingResultItem::setPriority(const int priority) {
    this->prio = priority;
}

void DictionaryDingResultItem::addToPriority(const int priority) {
    this->prio += priority;
}

bool DictionaryDingResultItem::operator<(const DictionaryDingResultItem& other) const {
    return !(priority() < other.priority());
}

