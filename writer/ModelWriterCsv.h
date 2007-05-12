//
// C++ Interface: vocablelistwriter
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MODELWRITERCSV_H
#define MODELWRITERCSV_H

#import "ModelWriterAbstract.h"
#import <QString>
class VocableListModel;
class Vocable;
/**
	@author Niko Sams <niko.sams@gmail.com>
 */
class ModelWriterCsv : public ModelWriterAbstract {
    public:
        ModelWriterCsv(const QString& fileName);

        bool write(VocableListModel* model);
        static QString vocableCsvString(Vocable *voc);
    private:
        static QString escape(QString str);
};

#endif
