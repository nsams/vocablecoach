//
// C++ Implementation: vocablelistwriter
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "VocableListWriter.h"
#include "VocableListModel.h"
#include <QFile>
#include <QStringList>

VocableListWriter::VocableListWriter(const QString& fileName)
	: m_fileName(fileName)
{
}


VocableListWriter::~VocableListWriter()
{
}

bool VocableListWriter::write(VocableListModel* model)
{
	QFile file(m_fileName);
	if(!file.open(QIODevice::WriteOnly)) return false;
	QStringList out;
	out << "<?xml version=\"1.0\"?>";
	out << "<!DOCTYPE kvtml SYSTEM \"kvoctrain.dtd\">";
	out << "<kvtml";
	out << "	encoding=\"UTF-8\"";
	out << "	generator=\"vocablecoach 0.0\"";
	out << "	cols=\"2\"";
    out << QString("	lines=\"%1\"").arg(model->rowCount());
	out << QString("	title=\"%1\"").arg(model->title());
    out << QString("	author=\"%1\"").arg(model->authors());
    out << QString("	foreignLanguage=\"%1\"").arg(model->foreignLanguage()); //fixme: not kvoctrain format
    out << QString("	nativeLanguage=\"%1\">").arg(model->nativeLanguage()); //fixme: not kvoctrain format
    for(int i=0;i<model->rowCount();i++)
	{
		Vocable* voc = model->vocable(i);
		out << QString("<e box=\"%1\" lesson=\"%2\">").arg(voc->box()).arg(voc->lesson());
		out << QString("	<o>%1</o>").arg(escape(voc->foreign()));
		QString lastQuery("");
		if(voc->lastQuery().isValid()) {
			lastQuery = " d=\""+QString::number(voc->lastQuery().toTime_t())+";0\"";
		}
		out << QString("	<t%1>%2</t>").arg(lastQuery).arg(escape(voc->native()));
		out << "</e>";
	}
	out << "</kvtml>";
	file.write(out.join("\n").toUtf8());
	return true;
}

QString VocableListWriter::escape(const QString &s)
{
	QString ret = s;
	ret.replace(QChar('&'), "&amp;"); //must be done first 
	ret.replace(QChar('<'), "&lt;");
	ret.replace(QChar('>'), "&gt;");

	return ret;
}
