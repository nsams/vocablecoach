//
// C++ Interface: VocableEditorTextEdit
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOCABLEEDITORTEXTEDIT_H
#define VOCABLEEDITORTEXTEDIT_H

#include <QTextEdit>

class QFocusEvent;
/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class VocableEditorTextEdit : public QTextEdit
{
Q_OBJECT
public:
    VocableEditorTextEdit(QWidget *parent = 0);
    ~VocableEditorTextEdit();

protected:
    void focusInEvent (QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
};

#endif
