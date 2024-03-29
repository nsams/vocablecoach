//
// C++ Interface: VocableListModelFiltered
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOCABLELISTMODELFILTERED_H
#define VOCABLELISTMODELFILTERED_H

#include <QAbstractItemModel>
#include <QList>
#include "Vocable.h"
#include "VocableQuiz.h"
#include <QStringList>
#include <QMap>

/**
	@author Niko Sams <niko.sams@gmail.com>
 */
class VocableListModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
        VocableListModel(QObject * parent = 0)
            : QAbstractItemModel(parent), m_nativeColumnWidth(0), m_foreignColumnWidth(0), m_lessonColumnWidth(0) { }
        ~VocableListModel();

        QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
		int columnCount ( const QModelIndex & parent ) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		Qt::ItemFlags flags ( const QModelIndex & index ) const;
//		bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
		bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
		bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
		QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &child) const;
        
		Vocable* vocable(const QModelIndex & index);
		Vocable* vocable(int row);
		void insertVocable(int position, Vocable* vocable);
		void appendVocable(Vocable* vocable);
        Vocable* randomVocable(Vocable::Direction direction, VocableQuiz::QuizType type, QStringList lessons=QStringList());
        QDateTime nextExpiredVocable(Vocable::Direction direction, VocableQuiz::QuizType type, QStringList lessons=QStringList());
		void clearVocables();
        
        int unlearnedCount(Vocable::Direction direction);
        int inBoxCount(int box, Vocable::Direction direction);

        QMap<int, QString> lessons() const;
        int getLessonNumber(const QString& lesson);
        QString getLessonByNumber(int i);
        void insertLesson(int number, const QString& lesson);

        QString title() const { return m_title; }
        QString authors() const { return m_authors; }
        QString nativeLanguage() const { return m_nativeLanguage; }
        QString foreignLanguage() const { return m_foreignLanguage; }
        QMap<QString, QVariant> nativeLanguageSettings() const { return m_nativeLanguageSettings; }
        QMap<QString, QVariant> foreignLanguageSettings() const { return m_foreignLanguageSettings; }
        QString nativeLanguageDict() const { return m_nativeLanguageDict; }
        QString foreignLanguageDict() const { return m_foreignLanguageDict; }
        void setTitle(const QString &title) { m_title = title; }
        void setAuthors(const QString &authors) { m_authors = authors; }
        void setNativeLanguage(const QString &nativeLanguage) { m_nativeLanguage = nativeLanguage; }
        void setForeignLanguage(const QString &foreignLanguage) { m_foreignLanguage = foreignLanguage; }
        void setNativeLanguageSettings(const QMap<QString, QVariant> &nativeLanguageSettings) { m_nativeLanguageSettings = nativeLanguageSettings; }
        void setForeignLanguageSettings(const QMap<QString, QVariant> &foreignLanguageSettings) { m_foreignLanguageSettings = foreignLanguageSettings; }
        void setNativeLanguageDict(const QString &nativeLanguageDict) { m_nativeLanguageDict = nativeLanguageDict; }
        void setForeignLanguageDict(const QString &foreignLanguageDict) { m_foreignLanguageDict = foreignLanguageDict; }

        int nativeColumnWidth() { return m_nativeColumnWidth; }
        int foreignColumnWidth() { return m_foreignColumnWidth; }
        int lessonColumnWidth() { return m_lessonColumnWidth; }
        void setNativeColumnWidth(int width) { m_nativeColumnWidth = width; }
        void setForeignColumnWidth(int width) { m_foreignColumnWidth = width; }
        void setLessonColumnWidth(int width) { m_lessonColumnWidth = width; }

        QStringList boxes();

        Vocable* createVocable();

	private:
		QList<Vocable*> m_vocableList;

        QString m_title;
        QString m_authors;
        QMap<int, QString> m_lessons;

        QString m_nativeLanguage;
        QString m_foreignLanguage;
        QMap<QString, QVariant> m_nativeLanguageSettings;
        QMap<QString, QVariant> m_foreignLanguageSettings;
        QString m_nativeLanguageDict;
        QString m_foreignLanguageDict;

        int m_nativeColumnWidth;
        int m_foreignColumnWidth;
        int m_lessonColumnWidth;

    signals:
        void vocableChanged();
};

#endif
