//
// C++ Interface: vocablelistwriter
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOCABLELISTWRITER_H
#define VOCABLELISTWRITER_H

#import <QString>
class VocableListModel;
/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class VocableListWriter{
public:
    VocableListWriter(const QString& fileName);

    ~VocableListWriter();
	bool write(VocableListModel* model);
private:
	QString m_fileName;
	QString escape(const QString &s);

};

#endif
