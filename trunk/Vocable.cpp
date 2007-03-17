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

