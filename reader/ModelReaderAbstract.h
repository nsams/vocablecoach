//
// C++ Interface: Abstract
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MODELREADERABSTRACT_H
#define MODELREADERABSTRACT_H
#include <QString>

class VocableListModel;
/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class ModelReaderAbstract{
public:
    ModelReaderAbstract(const QString& filename);
    virtual ~ModelReaderAbstract();
    virtual bool read(VocableListModel* model) = 0;
    virtual bool isValidFile() = 0;
protected:
    QString m_fileName;
};

#endif
