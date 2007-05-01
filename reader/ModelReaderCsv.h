//
// C++ Interface: ModelReaderCsv
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MODELREADERCSV_H
#define MODELREADERCSV_H

#include "ModelReaderAbstract.h"

/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class ModelReaderCsv : public ModelReaderAbstract
{
public:
    ModelReaderCsv(const QString& filename);
    bool read(VocableListModel* model);
    bool isValidFile();

};

#endif
