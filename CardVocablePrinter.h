//
// C++ Interface: CardVocablePrinter
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CARDVOCABLEPRINTER_H
#define CARDVOCABLEPRINTER_H

#include "AbstractVocablePrinter.h"

/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class CardVocablePrinter : public AbstractVocablePrinter
{
public:
    CardVocablePrinter(VocableListModel *model, QPrinter *printer);

    void printPage(int index, QPainter &painter);
    int pageCount();

private:
    static const int padding = 5;
    int rowHeight(int row, QPainter& painter);
};

#endif
