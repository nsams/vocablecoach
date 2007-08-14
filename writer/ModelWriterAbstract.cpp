//
// C++ Implementation: Abstract
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ModelWriterAbstract.h"
#include "VocableListModel.h"

ModelWriterAbstract::ModelWriterAbstract(const QString& filename)
    : m_fileName(filename)
{
}


ModelWriterAbstract::~ModelWriterAbstract()
{
}


