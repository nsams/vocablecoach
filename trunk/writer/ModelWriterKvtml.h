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
#ifndef MODELWRITERKVTML_H
#define MODELWRITERKVTML_H

#import "ModelWriterAbstract.h"
#import <QString>
class VocableListModel;
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class ModelWriterKvtml : public ModelWriterAbstract {
public:
    ModelWriterKvtml(const QString& fileName);

    bool write(VocableListModel* model);

private:
	QString escape(const QString &s);

};

#endif
