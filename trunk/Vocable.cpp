#include "Vocable.h"
#include "VocableListModel.h"

Vocable::Vocable(VocableListModel* model, const QString& native, const QString& foreign)
    : QObject(0), m_model(model), m_native(native), m_foreign(foreign), m_lessonNumber(0)
{
    m_queryCount[NativeToForeign] = 0;
    m_queryCount[ForeignToNative] = 0;
    m_badCount[NativeToForeign] = 0;
    m_badCount[ForeignToNative] = 0;
    m_box[NativeToForeign] = 0;
    m_box[ForeignToNative] = 0;
}

void Vocable::setNative(const QString& native)
{
	m_native = native;
    emit vocableChanged();
}
void Vocable::setForeign(const QString& foreign)
{
	m_foreign = foreign;
    emit vocableChanged();
}

void Vocable::setBox(Direction direction, int box)
{
    m_box[direction] = box;
    emit vocableChanged();
}
void Vocable::setLastQuery(Direction direction, const QDateTime& lastQuery)
{
    m_lastQuery[direction] = lastQuery;
    emit vocableChanged();
}

void Vocable::setLesson(const QString& lesson)
{
    m_lessonNumber = m_model->getLessonNumber(lesson);
    emit vocableChanged();
}

void Vocable::setLessonNumber(int i)
{
    m_lessonNumber = i;
    emit vocableChanged();
}

QString Vocable::lesson() const
{
    if (!m_lessonNumber) return "";
    return m_model->getLessonByNumber(m_lessonNumber);
}
int Vocable::lessonNumber() const
{
    return m_lessonNumber;
}
QDateTime Vocable::expireDate(Direction direction) const
{
    if (m_box[direction] == 2) {
        return m_lastQuery[direction].addSecs(13*60);
    } else if(m_box[direction]==1) {
        return m_lastQuery[direction].addSecs(16);
    } else if(m_box[direction]==0) {
        return QDateTime::currentDateTime();
    } else {
        return m_lastQuery[direction].addSecs(60*60*24);
    }
}

bool Vocable::isExpired(Direction direction) const
{
    if (expireDate(direction) < QDateTime::currentDateTime())
        return true;
    else
        return false;
}

int Vocable::queryCount(Direction direction) const
{
    return m_queryCount[direction];
}

void Vocable::setQueryCount(Direction direction, int queryCount)
{
    m_queryCount[direction] = queryCount;
}

int Vocable::badCount(Direction direction) const
{
    return m_badCount[direction];
}

void Vocable::setBadCount(Direction direction, int badCount)
{
    m_badCount[direction] = badCount;
}
QPair<QString, QString> Vocable::text(Direction direction)
{
    QPair<QString, QString> ret;
    if (direction == NativeToForeign) {
        ret.first = native();
        ret.second = foreign();
    } else if (direction == ForeignToNative) {
        ret.first = foreign();
        ret.second = native();
    } else {
        qFatal("invalid direction");
    }
    return ret;
}

QString Vocable::boxShortString(Direction direction) const {
    int i = box(direction);
    if (i == 0) {
        return tr("unlearned");
    } else if (i == 1) {
        return tr("ultra-shortterm");
    } else if (i == 2) {
        return tr("shortterm");
    } else {
        return tr("Box %1").arg(i-2);
    }
}

QString Vocable::lastQueryString() const
{
    return QString("%1/%2").arg(lastQuery(Vocable::NativeToForeign).isValid() ?
            lastQuery(Vocable::NativeToForeign).toString(Qt::LocaleDate)
    : tr("none"))
            .arg(lastQuery(Vocable::ForeignToNative).isValid() ?
            lastQuery(Vocable::ForeignToNative).toString(Qt::LocaleDate)
    : tr("none"));
}
