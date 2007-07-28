#include "Vocable.h"
#include "VocableListModel.h"

Vocable::Vocable(VocableListModel* model, const QString& native, const QString& foreign, int box)
    : QObject(0), m_model(model), m_native(native), m_foreign(foreign), m_lessonNumber(0), m_box(box),
        m_queryCount(0), m_badCount(0)
{
    
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

void Vocable::setBox(int box)
{
	m_box = box;
    emit vocableChanged();
}
void Vocable::setLastQuery(const QDateTime& lastQuery)
{
	m_lastQuery = lastQuery;
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
QDateTime Vocable::expireDate() const
{
    if (m_box == 2) {
        return m_lastQuery.addSecs(13*60);
    } else if(m_box==1) {
        return m_lastQuery.addSecs(16);
    } else if(m_box==0) {
        return QDateTime::currentDateTime();
    } else {
        return m_lastQuery.addSecs(60*60*24);
    }
}

bool Vocable::isExpired() const
{
    if (expireDate() < QDateTime::currentDateTime())
        return true;
    else
        return false;
}

int Vocable::queryCount() const
{
    return m_queryCount;
}

void Vocable::setQueryCount(int queryCount)
{
    m_queryCount = queryCount;
}

int Vocable::badCount() const
{
    return m_badCount;
}

void Vocable::setBadCount(int badCount)
{
    m_badCount = badCount;
}
