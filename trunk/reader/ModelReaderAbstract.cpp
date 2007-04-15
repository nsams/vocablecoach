//
// C++ Implementation: Abstract
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ModelReaderAbstract.h"
#include "VocableListModel.h"

ModelReaderAbstract::ModelReaderAbstract(const QString& filename)
    : m_fileName(filename)
{
}


ModelReaderAbstract::~ModelReaderAbstract()
{
}


