//
// C++ Interface: Abstract
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MODELWRITERABSTRACT_H
#define MODELWRITERABSTRACT_H
#include <QString>

class VocableListModel;
/**
	@author Niko Sams <niko.sams@gmail.com>
 */
class ModelWriterAbstract{
    public:
        ModelWriterAbstract(const QString& filename);
        virtual ~ModelWriterAbstract();
        virtual bool write(VocableListModel* model) = 0;
    protected:
        QString m_fileName;
};

#endif
