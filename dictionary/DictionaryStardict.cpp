//
// C++ Implementation: DictionaryStardict
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
// Based on QStarDict Copyright (C) 2007 Alexander Rodin                                           *
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dictionary/DictionaryStardict.h"
#include "dictionary/stardictlib/lib.h"
#include "dictionary/stardictlib/file.hpp"
#include "ui/ui_SettingsStardict.h"
#include <QDir>
#include <QDebug>

const int DictionaryStardict::MaxFuzzy(24);

DictionaryStardict::DictionaryStardict(const QMap<QString, QVariant>& settings, QObject *parent)
 : DictionaryAbstract(settings, parent)
{
    m_sdLibs = 0;
#ifdef Q_OS_UNIX
    m_dictDirs << "/usr/share/stardict/dic";
#else
    m_dictDirs << QCoreApplication::applicationDirPath() + "/dic";
#endif // Q_OS_UNIX

    m_dictDirs << QDir::homePath() + "/.stardict/dic";

    _loadDict();
}


DictionaryStardict::~DictionaryStardict()
{
}

void DictionaryStardict::_loadDict()
{
    QString dictName = m_settings["dict"].toString();

    delete m_sdLibs;
    m_sdLibs = new Libs;

    foreach(QString dictDir, m_dictDirs) {
        QString dictFile = _whereDict(dictName, dictDir);
        if (! dictFile.isEmpty())
            m_sdLibs->load_dict(QDir::toNativeSeparators(dictFile).toUtf8().data());
    }
}

QString DictionaryStardict::_whereDict(const QString &name, const QString &path)
{
    if (QFile::exists(path + "/" + name + ".ifo"))
        return path + "/" + name + ".ifo";

    QStringList dirs = QDir(path).entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for (QStringList::const_iterator i = dirs.begin(); i != dirs.end(); ++i)
    {
        QString result = _whereDict(name, path + "/" + *i);
        if (! result.isEmpty())
            return result;
    }

    return QString();
}

void DictionaryStardict::lookupWord(const QString& word)
{
    DictionaryAbstract::lookupWord(word);

    SearchResultList resultList;
    _lookupWithFuzzy(word.toUtf8().data() + std::string("*"), resultList);
    for (SearchResultList::const_iterator i = resultList.begin(); i != resultList.end(); ++i) {
//         if (! result.contains(i->def)) {
                QString simplifiedStr = i->def.simplified();

                SearchResultList resultList;
                std::string query;

                _simpleLookup(simplifiedStr.toUtf8().data(), resultList);
                foreach(SearchResult r, resultList) {
                    QStringList lines = r.exp.split("\n");
                    if (lines.count() >= 3) {
                        foreach(QString t, lines[2].split(";")) {
                            QPair<QString, QString> row;
                            row.first = r.def;
                            row.second = t.trimmed();
                            m_results << row;
                        }
                    }
                }
    }

    emit done(false);
}

void DictionaryStardict::_lookupWithFuzzy(const std::string &str, SearchResultList &resultList) // taken from sdcv
{
    gchar *fuzzy_res[MaxFuzzy];
    if (! m_sdLibs->LookupWithFuzzy(str.c_str(), fuzzy_res, MaxFuzzy))
        return ;

    for (gchar **p = fuzzy_res, **end = fuzzy_res + MaxFuzzy;
         p != end && *p; ++p)
    {
        _simpleLookup(*p, resultList);
        g_free(*p);
    }
}
void DictionaryStardict::_simpleLookup(const std::string &str, SearchResultList &resultList) // taken from sdcv
{
    glong ind;
    for (int idict = 0; idict < m_sdLibs->ndicts(); idict++) {
        if (m_sdLibs->SimpleLookupWord(str.c_str(), ind, idict)) {
            resultList.push_back(
                                 SearchResult(QString::fromUtf8(m_sdLibs->poGetWord(ind, idict)),
                                              _parse_data(m_sdLibs->poGetWordData(ind, idict))));
        }
    }
}

QString DictionaryStardict::_parse_data(const char *data) // taken from sdcv
{
    if (!data)
        return "";

    QString res;
    guint32 data_size, sec_size = 0;
    gchar *m_str;
    const gchar *p = data;
    data_size = *((guint32 *)p);
    p += sizeof(guint32);
    while (guint32(p - data) < data_size)
    {
        switch (*p++)
        {
            case 'm':
                case 'l':  //need more work...
            case 'g':
                sec_size = strlen(p);
                if (sec_size)
                {
                    res += "\n";
                    m_str = g_strndup(p, sec_size);
                    res += QString::fromUtf8(m_str);
                    g_free(m_str);
                }
                ++sec_size;
                break;
            case 'x':
                sec_size = strlen(p);
                if (sec_size)
                {
                    res += "\n";
                    m_str = g_strndup(p, sec_size);
//                    res += xdxf2html(m_str);
                    res += QString::fromUtf8(m_str);
                    g_free(m_str);
                }
                ++sec_size;
                break;
            case 't':
                sec_size = strlen(p);
                if (sec_size)
                {
                    res += "\n";
                    m_str = g_strndup(p, sec_size);
                    res += "[" + QString::fromUtf8(m_str) + "]";
                    g_free(m_str);
                }
                ++sec_size;
                break;
            case 'y':
                sec_size = strlen(p);
                ++sec_size;
                break;
            case 'W':
            case 'P':
                sec_size = *((guint32 *)p);
                sec_size += sizeof(guint32);
                break;
        }
        p += sec_size;
    }
    return res;
}
QStringList DictionaryStardict::_avialableDicts() const
{
    QStringList result;

    for (QStringList::ConstIterator dictDir = m_dictDirs.begin(); dictDir != m_dictDirs.end(); ++dictDir)
        result << _findDicts(*dictDir);

    return result;
}

QStringList DictionaryStardict::_findDicts(const QString &dir) const
{
    QFileInfoList result = QDir(dir).entryInfoList(QStringList("*.ifo"), QDir::Files | QDir::AllDirs |
            QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList dicts;

    for (QFileInfoList::const_iterator i = result.begin(); i != result.end(); ++i)
    {
        if (i->isDir())
            dicts << _findDicts(i->filePath());
        else
            dicts << QString(i->fileName()).remove(QRegExp("\\.ifo$"));
    }

    return dicts;
}

QMap<QString, QVariant> DictionaryStardict::editSettings(QWidget* parent)
{
    QDialog dialog(parent);;
    Ui::DictionarySettingsStardict ui;
    ui.setupUi(&dialog);

    QStringList languages;
    foreach(QString dict, _avialableDicts()) {
        ui.languageComboBox->addItem(dict, dict);
    }
    ui.languageComboBox->setCurrentIndex(ui.languageComboBox->findData(m_settings["dict"]));

    if (dialog.exec() == QDialog::Accepted) {
        m_settings["dict"] = ui.languageComboBox->itemData(ui.languageComboBox->currentIndex()).toString();
    }
    _loadDict(); //reload
    return m_settings;
}

QPair<QString, QString> DictionaryStardict::headerText()
{
    QPair<QString, QString> ret;
    ret.first = "";
    ret.second = "";
    return ret;
}
