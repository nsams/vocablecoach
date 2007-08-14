//
// C++ Interface: AbstactVocablePrinter
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ABSTACTVOCABLEPRINTER_H
#define ABSTACTVOCABLEPRINTER_H

class VocableListModel;
class QPainter;
class QPrinter;
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class AbstractVocablePrinter
{
public:
    AbstractVocablePrinter(VocableListModel *model, QPrinter *printer);
    virtual ~AbstractVocablePrinter();

    virtual void printPage(int index, QPainter &painter) = 0;
    virtual int pageCount() = 0;

protected:
    VocableListModel *m_vocableListModel;
    QPrinter *m_printer;
};

#endif
