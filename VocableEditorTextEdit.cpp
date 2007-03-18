//
// C++ Implementation: VocableEditorTextEdit
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "VocableEditorTextEdit.h"
#include <QFocusEvent>
#include <QTextEdit>

VocableEditorTextEdit::VocableEditorTextEdit(QWidget *parent)
 : QTextEdit(parent)
{
}


VocableEditorTextEdit::~VocableEditorTextEdit()
{
}

void VocableEditorTextEdit::focusInEvent(QFocusEvent* event)
{
    QTextEdit::focusInEvent(event);
    selectAll();
}

void VocableEditorTextEdit::focusOutEvent(QFocusEvent* event)
{
    QTextEdit::focusOutEvent(event);
//    moveCursor(QTextEdit::MoveHome);
}

