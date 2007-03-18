#ifndef VOCABLE_H
#define VOCABLE_H

#include <QString>
#include <QDateTime>
class VocableListModel;

class Vocable
{
public:
	Vocable(VocableListModel* model, const QString& native = QString(), const QString& foreign = QString(), int box = 0);
    QString native() { return m_native; }
    QString foreign() { return m_foreign; }
    int box() { return m_box; }
	QDateTime lastQuery() { return m_lastQuery; }
    QString lession() { return m_lession; }
    
	void setNative(const QString& native);
	void setForeign(const QString& foreign);
	void setBox(int box);
	void setLastQuery(const QDateTime& lastQuery);
    void setLession(const QString& lession);
private:
	VocableListModel* m_model;
	QString m_native;
	QString m_foreign;
    QString m_lession;
	int m_box;
	QDateTime m_lastQuery;

};
#endif
