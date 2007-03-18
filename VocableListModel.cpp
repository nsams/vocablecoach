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
            return voc->lession();
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
			return tr("Native");
		else if (section==1)
			return tr("Foreign");
		else if (section==2)
			return tr("Batch");
        else if (section==3)
            return tr("Lession");
        else if (section==4)
			return tr("Last Query");
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
        else if(index.column()==3) voc->setLession(value.toString());
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
Vocable* VocableListModel::randomVocable(bool onlyNew)
{
	srand( time(NULL) );
	QList<Vocable*> vocables;
	if(onlyNew)
	{
		foreach(Vocable* v, m_vocableList)
		{
			if(v->box()==2 && v->lastQuery().secsTo(QDateTime::currentDateTime()) > 13*60)
				vocables << v;
		}
		if(vocables.count()==0)
		{
			foreach(Vocable* v, m_vocableList)
			{
				if(v->box()==1 && v->lastQuery().secsTo(QDateTime::currentDateTime()) > 18)
					vocables << v;
			}
		}
		if(vocables.count()==0)
		{
			foreach(Vocable* v, m_vocableList)
			{
				if(v->box()==0)
					vocables << v;
			}
		}
	}
	else
	{
		foreach(Vocable* v, m_vocableList)
		{
			if(v->box()>2 && v->lastQuery().secsTo(QDateTime::currentDateTime()) > 60*60*24)
				vocables << v;
		}
	}
	if(vocables.count()==0) {
		return 0;
	}
	int row = rand() % vocables.count();
	return vocables[row];
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
QStringList VocableListModel::getUsedLessionsList()
{
    QStringList ret;
    foreach(Vocable* voc, m_vocableList) {
        if(!ret.contains(voc->lession()) && !voc->lession().isEmpty()) {
            ret << voc->lession();
        }
    }
    return ret;
}
