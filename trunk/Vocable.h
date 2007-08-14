#ifndef VOCABLE_H
#define VOCABLE_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QHash>
#include <QPair>

class VocableListModel;

class Vocable : public QObject
{
    Q_OBJECT
public:
	Vocable(VocableListModel* model, const QString& native = QString(), const QString& foreign = QString());

    enum Direction { NativeToForeign, ForeignToNative, Random };

    virtual ~Vocable() {}
    QString native() const { return m_native; }
    QString foreign() const { return m_foreign; }
    QPair<QString, QString> text(Direction direction);
    int box(Direction direction) const { return m_box[direction]; }
    QString boxShortString(Direction direction) const;
    QDateTime lastQuery(Direction direction) const { return m_lastQuery[direction]; }
    QDateTime expireDate(Direction direction) const;
    QString lesson() const;
    int lessonNumber() const;
    bool isExpired(Direction direction) const;
    int queryCount(Direction direction) const;
    int badCount(Direction direction) const;
    
	void setNative(const QString& native);
	void setForeign(const QString& foreign);
    void setBox(Direction direction, int box);
    void setLastQuery(Direction direction, const QDateTime& lastQuery);
    void setLesson(const QString& lesson);
    void setLessonNumber(int i);
    void setQueryCount(Direction direction, int queryCount);
    void setBadCount(Direction direction, int badCount);
    
private:
	VocableListModel* m_model;
	QString m_native;
	QString m_foreign;
    int m_lessonNumber;
    QHash<Direction, int> m_box;
    QHash<Direction, QDateTime> m_lastQuery;
    QHash<Direction, int> m_queryCount;
    QHash<Direction, int> m_badCount;

signals:
    void vocableChanged();
};
#endif
