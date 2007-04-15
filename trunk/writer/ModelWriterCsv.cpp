//
// C++ Implementation: ModelWriterCsv
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ModelWriterCsv.h"
#include "../VocableListModel.h"
#include <QFile>

ModelWriterCsv::ModelWriterCsv(const QString& fileName)
    : ModelWriterAbstract(fileName)
{
}


bool ModelWriterCsv::write(VocableListModel* model)
{
    QFile file(m_fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(QString(model->foreignLanguage()+";"+model->nativeLanguage()+";"+QObject::tr("Box")+"\n").toUtf8());

    for(int i=0;i<model->rowCount();i++)
    {
        Vocable* voc = model->vocable(i);
        QString out = "\"" + voc->foreign().replace("\n", "\\n").replace("\"", "\\\"") + "\";\"" + voc->native().replace("\n", "\\n").replace("\"", "\\\"") + "\";" + QString("%1").arg(voc->box()) + "\n";
        file.write(out.toUtf8());
    }
    return true;
}
