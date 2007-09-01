//
// C++ Implementation: ModelWriterKvtml
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ModelWriterKvtml.h"
#include "VocableListModel.h"
#include <QFile>
#include <QStringList>
#include <QDebug>

ModelWriterKvtml::ModelWriterKvtml(const QString& fileName)
    : ModelWriterAbstract(fileName)
{
}


bool ModelWriterKvtml::write(VocableListModel* model)
{
    QFile file(m_fileName);
    if(!file.open(QIODevice::WriteOnly)) return false;
    QStringList out;
    out << "<?xml version=\"1.0\"?>";
    out << "<!DOCTYPE kvtml SYSTEM \"kvoctrain.dtd\">";
    out << "<kvtml";
    out << "    encoding=\"UTF-8\"";
    out << "    generator=\"vocablecoach 0.0\"";
    out << "    cols=\"2\"";
    out << QString("    lines=\"%1\"").arg(model->rowCount());
    out << QString("    title=\"%1\"").arg(model->title());
    out << QString("    author=\"%1\">").arg(model->authors());

    out << "<languages>";
    out << QString("    <language type=\"native\" dict=\"%1\">%2")
            .arg(escape(model->nativeLanguageDict()))
            .arg(escape(model->nativeLanguage()));
    {
        QMapIterator<QString, QVariant> i(model->nativeLanguageSettings());
        while (i.hasNext()) {
            i.next();
            out << QString("        <dictSetting key=\"%1\" value=\"%2\" />")
                    .arg(escape(i.key()))
                    .arg(escape(i.value().toString()));
        }
        out << "    </language>";
    }

    out << QString("    <language type=\"foreign\" dict=\"%1\">%2")
            .arg(escape(model->foreignLanguageDict()))
            .arg(escape(model->foreignLanguage()));
    {
        QMapIterator<QString, QVariant> i(model->foreignLanguageSettings());
        while (i.hasNext()) {
            i.next();
            out << QString("        <dictSetting key=\"%1\" value=\"%2\" />")
                    .arg(escape(i.key()))
                    .arg(escape(i.value().toString()));
        }
        out << "    </language>";
    }
    out << "</languages>";

    out << QString("<lesson width=\"%1\">").arg(model->lessonColumnWidth());
    QMap<int, QString> lessons = model->lessons();
    QMap<int, QString>::const_iterator i;
    for (i = lessons.constBegin(); i != lessons.constEnd(); ++i) {
        out << QString("    <desc no=\"%1\">%2</desc>").arg(i.key()).arg(escape(i.value()));
    }
    out << "</lesson>";

    for (int i=0;i<model->rowCount();i++) {

        Vocable* voc = model->vocable(i);

        out << QString("<e m=\"%1\">").arg(voc->lessonNumber());

        QString width("");
        if (i == 0) width = QString(" width=\"%1\"").arg(model->nativeColumnWidth());
        out << QString("    <o%1>%2</o>").arg(width).arg(escape(voc->native()));

        QString lastQuery("");
        if (voc->lastQuery(Vocable::NativeToForeign).isValid()) {
            lastQuery = " d=\""+QString::number(voc->lastQuery(Vocable::NativeToForeign).toTime_t())+";0\"";
        }
        width = "";
        if (i == 0) width = QString(" width=\"%1\"").arg(model->foreignColumnWidth());
        out << QString("    <t%1%2 c=\"%3;%4\" b=\"%5;%6\" g=\"%7;%8\">%9</t>").arg(lastQuery).arg(width)
                .arg(voc->queryCount(Vocable::NativeToForeign)).arg(voc->queryCount(Vocable::ForeignToNative))
                .arg(voc->badCount(Vocable::NativeToForeign)).arg(voc->badCount(Vocable::ForeignToNative))
                .arg(voc->box(Vocable::NativeToForeign)).arg(voc->box(Vocable::ForeignToNative))
                .arg(escape(voc->foreign()));
        out << "</e>";
    }
    out << "</kvtml>\n";
    file.write(out.join("\n").toUtf8());
    return true;
}

QString ModelWriterKvtml::escape(const QString &s)
{
    QString ret = s;
    ret.replace(QChar('&'), "&amp;"); //must be done first 
    ret.replace(QChar('<'), "&lt;");
    ret.replace(QChar('>'), "&gt;");

    return ret;
}
