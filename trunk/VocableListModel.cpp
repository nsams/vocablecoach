#include "VocableListModel.h"
#include "Vocable.h"
#include "ImportPauker.h"
#include <QDebug>
#include <stdlib.h>
#include <zlib.h>
#include <QMessageBox>
#include <QXmlSimpleReader>

QVariant VocableListModel::data ( const QModelIndex & index, int role ) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	if (index.row() >= m_vocableList.count()) {
		return QVariant();
	}

	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		Vocable* voc = m_vocableList.at(index.row());
		if(index.column()==0) {
			return voc->native();
		} else if(index.column()==1)
			return voc->foreign();
		else if(index.column()==2)
			return voc->box();
        else if(index.column()==3)
            return voc->lesson();
        else if(index.column()==4)
			return voc->lastQuery().toString(Qt::LocaleDate);
		else
			return QVariant();
	} else {
		return QVariant();
	}
}

int VocableListModel::rowCount ( const QModelIndex & /*parent*/ ) const
{
	return m_vocableList.count();
}
int VocableListModel::columnCount ( const QModelIndex & /*parent*/ ) const
{
	return 5;
}
QModelIndex VocableListModel::index(int row, int column, const QModelIndex & /*parent*/) const {
	return createIndex(row, column);
}
QModelIndex VocableListModel::parent(const QModelIndex &/*child*/) const {
	return QModelIndex();
}
QVariant VocableListModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
	if(orientation==Qt::Horizontal && role == Qt::DisplayRole) {
		if(section==0)
			return nativeLanguage();
		else if (section==1)
			return foreignLanguage();
		else if (section==2)
			return tr("box");
        else if (section==3)
            return tr("lesson");
        else if (section==4)
			return tr("last query");
	}
	if(orientation==Qt::Vertical && role == Qt::DisplayRole) {
		return QString("%1").arg(section+1);
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags VocableListModel::flags ( const QModelIndex & index ) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index)/* | Qt::ItemIsEditable*/;
}

bool VocableListModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
	if (index.isValid() && role == Qt::EditRole) {
		Vocable* voc = m_vocableList.at(index.row());
		if(index.column()==0) voc->setForeign(value.toString());
		else if(index.column()==1) voc->setNative(value.toString());
		else if(index.column()==2) voc->setBox(value.toInt());
        else if(index.column()==3) voc->setLesson(value.toString());
		else return false;
		emit dataChanged(index, index);
		return true;
	}
	return false;

}

bool VocableListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	Q_UNUSED (parent);
	beginInsertRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; ++row) {
		Vocable* newVoc = new Vocable(this);
		m_vocableList.insert(position, newVoc);
	}

	endInsertRows();
	return true;
}
void VocableListModel::insertVocable(int position, Vocable* vocable)
{
	beginInsertRows(QModelIndex(), position, position);

	m_vocableList.insert(position, vocable);
	
	endInsertRows();
}

void VocableListModel::appendVocable(Vocable* vocable)
{
	insertVocable(m_vocableList.count(), vocable);
}

bool VocableListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	Q_UNUSED (parent);
	beginRemoveRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; ++row) {
		Vocable* voc = m_vocableList.at(position);
		m_vocableList.removeAt(position);
		delete voc;
	}

	endRemoveRows();

	return true;
}

Vocable* VocableListModel::vocable(const QModelIndex & index)
{
	if(!index.isValid()) return 0;

	return m_vocableList.at(index.row());
}

Vocable* VocableListModel::vocable(int row)
{
	return m_vocableList.at(row);
}
Vocable* VocableListModel::randomVocable(QuizType type, QStringList lessons)
{
	srand( time(NULL) );
	QList<Vocable*> vocables;
    if (type == New || type == All)
	{
		foreach(Vocable* v, m_vocableList)
		{
			if(v->box()==2 && v->isExpired()
               && (lessons.empty() || lessons.contains(v->lesson())))
				vocables << v;
		}
		if(vocables.count()==0)
		{
			foreach(Vocable* v, m_vocableList)
			{
                if(v->box()==1 && v->isExpired()
                    && (lessons.empty() || lessons.contains(v->lesson())))
					vocables << v;
			}
		}
		if(vocables.count()==0)
		{
			foreach(Vocable* v, m_vocableList)
			{
                if(v->box()==0
                   && (lessons.empty() || lessons.contains(v->lesson())))
					vocables << v;
			}
		}
	}
    if (type == Expired || type == All)
	{
		foreach(Vocable* v, m_vocableList)
		{
            if(v->box()>2 && v->isExpired()
                && (lessons.empty() || lessons.contains(v->lesson())))
				vocables << v;
		}
	}

	if(vocables.count()==0) {
		return 0;
	}
	int row = rand() % vocables.count();
	return vocables[row];
}


QDateTime VocableListModel::nextExpiredVocable(QuizType type, QStringList lessons)
{
    QDateTime ret;

    foreach(Vocable* v, m_vocableList)
    {
        if(v->box() > 2 && type == New) continue;
        if(v->box() <= 2 && type == Expired) continue;
        if(!lessons.empty() && !lessons.contains(v->lesson())) continue;
        if(v->isExpired()) continue;
        if(ret.isValid()) ret = v->expireDate();

        if(ret < v->expireDate()) ret = v->expireDate();
    }
    return ret;
}


void VocableListModel::importFile(QString fileName)
{
	gzFile file;
	file = gzopen (fileName.toUtf8().data(), "rb");
	if(!file) {
		QMessageBox::critical(0, tr("import"), tr("Can't open file"));
		return;
	}
	QByteArray input;
	char buffer[1024];
	QByteArray inputData;
	while(int readBytes =  gzread (file, buffer, 1024))
	{
		input.append(QByteArray(buffer, readBytes));
	}
	gzclose(file);
	
	QXmlSimpleReader xmlReader;
	QXmlInputSource *source = new QXmlInputSource;
	source->setData(input);

	clearVocables();

	ImportPauker *handler = new ImportPauker(this);
	xmlReader.setContentHandler(handler);
	xmlReader.setErrorHandler(handler);
	
	bool ok = xmlReader.parse(source);

	if (!ok) {
		QMessageBox::critical(0, tr("import"), tr("Invalid XML-File"));
		clearVocables();
	}
}

void VocableListModel::clearVocables()
{
	qDeleteAll(m_vocableList);
	m_vocableList.clear();
	emitVocableChanged();

	reset();
}
void VocableListModel::insertLesson(int number, const QString& lesson)
{
    m_lessons.insert(number, lesson);
}
QMap<int, QString> VocableListModel::lessons() const
{
    return m_lessons;
}
int VocableListModel::getLessonNumber(const QString& lesson)
{
    QMap<int, QString>::const_iterator i;
    for (i = m_lessons.constBegin(); i != m_lessons.constEnd(); ++i) {
        if (*i == lesson) {
            return i.key();
        }
    }
    int max = 1;
    foreach(int i, m_lessons.keys()) {
        max = qMax(max, i);
    }
    m_lessons.insert(max+1, lesson);
    return max+1;
}
QString VocableListModel::getLessonByNumber(int i)
{
    return m_lessons.value(i);
}
void VocableListModel::emitVocableChanged()
{
    m_modified = true;
    emit vocableChanged();
}
