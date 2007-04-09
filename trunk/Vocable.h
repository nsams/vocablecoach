#ifndef VOCABLE_H
#define VOCABLE_H

#include <QString>
#include <QDateTime>
class VocableListModel;

class Vocable
{
public:
	Vocable(VocableListModel* model, const QString& native = QString(), const QString& foreign = QString(), int box = 0);
    QString native() const { return m_native; }
    QString foreign() const { return m_foreign; }
    int box() const { return m_box; }
    QDateTime lastQuery() const { return m_lastQuery; }
    QDateTime expireDate() const;
    QString lesson() const { return m_lesson; }
    bool isExpired() const;
    
	void setNative(const QString& native);
	void setForeign(const QString& foreign);
	void setBox(int box);
	void setLastQuery(const QDateTime& lastQuery);
    void setLesson(const QString& lesson);
private:
	VocableListModel* m_model;
	QString m_native;
	QString m_foreign;
    QString m_lesson;
	int m_box;
	QDateTime m_lastQuery;

};
#endif
