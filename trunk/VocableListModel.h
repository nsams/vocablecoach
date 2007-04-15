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
		VocableListModel(QObject * parent = 0) : QAbstractItemModel(parent) { }
		~VocableListModel() { qDeleteAll(m_vocableList); }

		QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
		int columnCount ( const QModelIndex & parent ) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		Qt::ItemFlags flags ( const QModelIndex & index ) const;
		bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
		bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
		bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
		QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &child) const;
        
		Vocable* vocable(const QModelIndex & index);
		Vocable* vocable(int row);
		void insertVocable(int position, Vocable* vocable);
		void appendVocable(Vocable* vocable);
        Vocable* randomVocable(QuizType type, QStringList lessons=QStringList());
        QDateTime nextExpiredVocable(QuizType type, QStringList lessons=QStringList());
		void clearVocables();
		void emitVocableChanged();

    	bool isModified() const { return m_modified; }
    	void setModified(bool modified) { m_modified = modified; }

        QMap<int, QString> lessons() const;
        int getLessonNumber(const QString& lesson);
        QString getLessonByNumber(int i);
        void insertLesson(int number, const QString& lesson);

        QString title() const { return m_title; }
        QString authors() const { return m_authors; }
        QString nativeLanguage() const { return m_nativeLanguage; }
        QString foreignLanguage() const { return m_foreignLanguage; }
        void setTitle(const QString &title) {
            m_title = title;
            emitVocableChanged();
        }
        void setAuthors(const QString &authors) { m_authors = authors; emitVocableChanged(); }
        void setNativeLanguage(const QString &nativeLanguage) { m_nativeLanguage = nativeLanguage; emitVocableChanged(); }
        void setForeignLanguage(const QString &foreignLanguage) { m_foreignLanguage = foreignLanguage; emitVocableChanged(); }
        

	private:
		QList<Vocable*> m_vocableList;
		bool m_modified;

        QString m_title;
        QString m_authors;
        QString m_nativeLanguage;
        QString m_foreignLanguage;
        QMap<int, QString> m_lessons;

	signals:
		void vocableChanged();
};

#endif
