//
// C++ Interface: DictionaryHttpAbstract
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICTIONARYHTTPABSTRACT_H
#define DICTIONARYHTTPABSTRACT_H

#include <dictionary/DictionaryAbstract.h>
class QHttp;
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class DictionaryHttpAbstract : public DictionaryAbstract
{
Q_OBJECT
public:
    DictionaryHttpAbstract(const QMap<QString, QVariant>& settings, QObject *parent = 0);
    void lookupWord(const QString& word);

private Q_SLOTS:
    virtual void processData(bool error) = 0;

protected:
    virtual QString host() = 0;
    QHttp* m_http;
    QString _cleanHtml(QString html);
};

#endif
