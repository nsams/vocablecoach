#include "Vocable.h"
#include "VocableListModel.h"

Vocable::Vocable(VocableListModel* model, const QString& native, const QString& foreign, int box)
	: m_model(model), m_native(native), m_foreign(foreign), m_box(box) { }

void Vocable::setNative(const QString& native)
{
	m_native = native;
// 	m_model->emitVocableChanged();
}
void Vocable::setForeign(const QString& foreign)
{
	m_foreign = foreign;
// 	m_model->emitVocableChanged();
}

void Vocable::setBox(int box)
{
	m_box = box;
// 	m_model->emitVocableChanged();
}
void Vocable::setLastQuery(const QDateTime& lastQuery)
{
	m_lastQuery = lastQuery;
// 	m_model->emitVocableChanged();
}

void Vocable::setLession(const QString& lession)
{
    m_lession = lession;
// 	m_model->emitVocableChanged();
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
