//
// C++ Implementation: AbstactVocablePrinter
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "AbstractVocablePrinter.h"
#include "VocableListModel.h"
#include <QPrinter>


AbstractVocablePrinter::AbstractVocablePrinter(VocableListModel *model, QPrinter *printer)
    : m_vocableListModel(model), m_printer(printer)
{
}

AbstractVocablePrinter::~AbstractVocablePrinter()
{
}


