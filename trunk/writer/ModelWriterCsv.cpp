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

    file.write(QString(ModelWriterCsv::escape(model->nativeLanguage())
            +"\t"+ ModelWriterCsv::escape(model->foreignLanguage())
            +"\t"+ ModelWriterCsv::escape(QObject::tr("Box"))+"\n").toUtf8());

    for(int i=0;i<model->rowCount();i++)
    {
        Vocable* voc = model->vocable(i);
        QString out = ModelWriterCsv::vocableCsvString(voc) + "\n";
        file.write(out.toUtf8());
    }
    return true;
}

QString ModelWriterCsv::vocableCsvString(Vocable *voc) {
    QString out = ModelWriterCsv::escape(voc->native())
            + "\t" + ModelWriterCsv::escape(voc->foreign())
            + "\t" + QString("%1").arg(voc->box(Vocable::NativeToForeign));
    return out;
}
QString ModelWriterCsv::escape(QString str)
{
    return str.replace("\\", "\\\\").replace("\n", "\\n").replace("\"", "\\\"");
}
