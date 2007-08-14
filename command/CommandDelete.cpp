//
// C++ Implementation: CommanDDelete
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command/CommandDelete.h"
#include "VocableListModel.h"
#include <QItemSelection>

CommandDelete::CommandDelete(VocableListModel* model, QList<int> rows, QUndoCommand * parent)
    : QUndoCommand(parent), m_vocableListModel(model), m_rows(rows)
{
    qSort( m_rows );

    foreach (int pos, m_rows) {
        m_vocables.append( m_vocableListModel->vocable(pos) );
    }

    if (m_rows.count() > 1) {
        setText(QObject::tr("Delete Vocables"));
    } else {
        setText(QObject::tr("Delete Vocable"));
    }
}


void CommandDelete::redo()
{
    for( int i = m_rows.count() - 1; i >= 0; i -= 1 ) {
        int pos = m_rows[i];
        m_vocableListModel->removeRows( pos, 1 );
    }
}

void CommandDelete::undo()
{
    for( int i = m_rows.count() - 1; i >= 0; i -= 1 ) {
        int pos = m_rows[i];
        Vocable* vocable = m_vocables[i];
        m_vocableListModel->insertVocable(pos, vocable);
    }
}
