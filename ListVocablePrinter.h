//
// C++ Interface: ListVocablePrinter
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTVOCABLEPRINTER_H
#define LISTVOCABLEPRINTER_H

#include "AbstractVocablePrinter.h"

/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class ListVocablePrinter : public AbstractVocablePrinter
{
public:
    ListVocablePrinter(VocableListModel *model, QPrinter *printer);

    void printPage(int index, QPainter &painter);
    int pageCount();

private:
    static const int padding = 5;
};

#endif
