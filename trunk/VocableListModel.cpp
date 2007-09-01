#include "VocableListModel.h"
#include "Vocable.h"
#include "reader/ModelReaderPauker.h"
#include <QDebug>
#include <QMessageBox>

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
		} else if(index.column()==1) {
			return voc->foreign();
        } else if(index.column()==2) {
            return voc->lesson();
        } else if(index.column()==3) {
            return QString("%1/%2").arg(voc->boxShortString(Vocable::NativeToForeign))
                    .arg(voc->boxShortString(Vocable::ForeignToNative));
        } else if(index.column()==4) {
            return voc->lastQueryString();
        } else if(index.column()==5) {
            return QString("%1/%2").arg(voc->queryCount(Vocable::NativeToForeign))
                    .arg(voc->queryCount(Vocable::ForeignToNative));
        } else if(index.column()==6) {
            return QString("%1/%2").arg(voc->badCount(Vocable::NativeToForeign))
                    .arg(voc->badCount(Vocable::ForeignToNative));
        } else {
			return QVariant();
        }
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
	return 7;
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
            return tr("lesson");
        else if (section==3)
            return tr("box");
        else if (section==4)
			return tr("last query");
        else if (section==5)
            return tr("query count");
        else if (section==6)
            return tr("bad count");
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
/*
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

}*/

bool VocableListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	Q_UNUSED (parent);
	beginInsertRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; ++row) {
		Vocable* newVoc = createVocable();
		m_vocableList.insert(position, newVoc);
	}

	endInsertRows();
	return true;
}
void VocableListModel::insertVocable(int position, Vocable* vocable)
{
	beginInsertRows(QModelIndex(), position, position);

	m_vocableList.insert(position, vocable);
    connect(vocable, SIGNAL(vocableChanged()), this, SIGNAL(vocableChanged()));
    emit vocableChanged();
	
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
//		Vocable* voc = m_vocableList.at(position);
		m_vocableList.removeAt(position);
// 		delete voc; //nicht direkt löschen, weil es für undo-funktionalität noch im speicher bleiben muss
	}
    
    emit vocableChanged();

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
Vocable* VocableListModel::randomVocable(Vocable::Direction direction, VocableQuiz::QuizType type, QStringList lessons)
{
	srand( time(NULL) );
	QList<Vocable*> vocables;
    if (type == VocableQuiz::New || type == VocableQuiz::All)
	{
		foreach(Vocable* v, m_vocableList)
		{
            if(v->box(direction)==2 && v->isExpired(direction)
               && (lessons.empty() || lessons.contains(v->lesson())))
				vocables << v;
		}
		if(vocables.count()==0)
		{
			foreach(Vocable* v, m_vocableList)
			{
                if(v->box(direction)==1 && v->isExpired(direction)
                    && (lessons.empty() || lessons.contains(v->lesson())))
					vocables << v;
			}
		}
		if(vocables.count()==0)
		{
			foreach(Vocable* v, m_vocableList)
			{
                if(v->box(direction)==0
                   && (lessons.empty() || lessons.contains(v->lesson())))
					vocables << v;
			}
		}
	}
    if (type == VocableQuiz::Expired || type == VocableQuiz::All)
	{
		foreach(Vocable* v, m_vocableList)
		{
            if(v->box(direction)>2 && v->isExpired(direction)
                && (lessons.empty() || lessons.contains(v->lesson())))
				vocables << v;
		}
	}

	if(vocables.count()==0) {
		return 0;
	}
	int row = qrand() % vocables.count();
	return vocables[row];
}


QDateTime VocableListModel::nextExpiredVocable(Vocable::Direction direction, VocableQuiz::QuizType type, QStringList lessons)
{
    QDateTime ret;

    foreach(Vocable* v, m_vocableList)
    {
        if(v->box(direction) > 2 && type == VocableQuiz::New) continue;
        if(v->box(direction) <= 2 && type == VocableQuiz::Expired) continue;
        if(!lessons.empty() && !lessons.contains(v->lesson())) continue;
        if(v->isExpired(direction)) continue;
        if(ret.isValid()) ret = v->expireDate(direction);

        if(ret < v->expireDate(direction)) ret = v->expireDate(direction);
    }
    return ret;
}

void VocableListModel::clearVocables()
{
	qDeleteAll(m_vocableList);
	m_vocableList.clear();
    emit vocableChanged();
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

Vocable* VocableListModel::createVocable()
{
    Vocable* voc = new Vocable(this);
    return voc; 
}
VocableListModel::~VocableListModel()
{
}


QStringList VocableListModel::boxes()
{
    int highestBox = 0;
    foreach(Vocable* voc, m_vocableList) {
        highestBox = qMax(highestBox, voc->box(Vocable::NativeToForeign));
        highestBox = qMax(highestBox, voc->box(Vocable::ForeignToNative));
    }
    QStringList ret;
    ret << tr("Unlearned");
    if (highestBox >= 1) {
        ret << tr("Ultra-Shortterm-Memory");
    }
    if (highestBox >= 2) {
        ret << tr("Shortterm-Memory");
    }
    for (int i=3; i <= highestBox; i++) {
        ret << tr("Box %1").arg(i-2);
    }
    return ret;
}

int VocableListModel::inBoxCount(int box, Vocable::Direction direction)
{
    int ret = 0;
    foreach(Vocable* voc, m_vocableList) {
        if (voc->box(direction) == box) {
            ret++;
        }
    }
    return ret;
}

int VocableListModel::unlearnedCount(Vocable::Direction direction)
{
    return inBoxCount(0, direction);
}
