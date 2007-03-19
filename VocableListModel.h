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
#include <QStringList>

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
        Vocable* randomVocable(bool onlyNew, QStringList lessions=QStringList());
		void importFile(QString fileName);
		void clearVocables();
		void emitVocableChanged() { m_modified = true; emit vocableChanged(); }

    	bool isModified() { return m_modified; }
    	void setModified(bool modified) { m_modified = modified; }
        QStringList getUsedLessionsList();
        
	private:
		QList<Vocable*> m_vocableList;
		bool m_modified;

	signals:
		void vocableChanged();
};

#endif
