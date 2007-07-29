#include "VocableList.h"
#include "VocableListModelFilter.h"
#include "VocableListModel.h"
#include <QDebug>

VocableList::VocableList(QWidget* parent)
    : QWidget(parent), m_vocableListModel(0)
{
    setupUi(this);

    m_filteredVocableListModel = new VocableListModelFilter(this);
    vocableEditorView->setModel(m_filteredVocableListModel);

    vocableEditorView->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(vocableEditorView, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(doubleClicked(QModelIndex)));
    
    connect(vocableEditorView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SIGNAL(selectionChanged(QItemSelection, QItemSelection)));

    boxSelectionCombo->addItem(tr("All"));
    boxSelectionCombo->setCurrentIndex(0);

    connect(boxSelectionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(boxFilterChanged(int)));
    boxFilterChanged(0);
    
    connect(filterLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(textFilterChanged(const QString&)));
}
VocableList::~VocableList()
{
}

void VocableList::addListAction(QAction* action)
{
    vocableEditorView->addAction(action);
}

Vocable* VocableList::currentVocable()
{
    QModelIndex currentIndex = vocableEditorView->selectionModel()->currentIndex();
    if(!currentIndex.isValid()) return 0;

    Vocable* vocable = m_vocableListModel->vocable(m_filteredVocableListModel->mapToSource(currentIndex));
    return vocable;
}

int VocableList::currentRow()
{
    QModelIndex currentIndex = m_filteredVocableListModel->mapToSource(vocableEditorView->selectionModel()->currentIndex());
    int currentRow;
    if (!currentIndex.isValid()) {
        currentRow = m_vocableListModel->rowCount()-1;
    } else {
        currentRow = -1;
    }
    return currentRow;
}

void VocableList::boxFilterChanged(int box)
{
    m_filteredVocableListModel->setBoxFilter(box-1);
}
void VocableList::textFilterChanged(const QString& text)
{
    m_filteredVocableListModel->setTextFilter(text);
}

void VocableList::setModel(VocableListModel* model)
{
    m_vocableListModel = model;
    m_filteredVocableListModel->setSourceModel(m_vocableListModel);

    if (m_vocableListModel->nativeColumnWidth() > 10) {
        vocableEditorView->setColumnWidth(0, m_vocableListModel->nativeColumnWidth());
    }
    if (m_vocableListModel->foreignColumnWidth() > 10) {
        vocableEditorView->setColumnWidth(1, m_vocableListModel->foreignColumnWidth());
    }
    if (m_vocableListModel->lessonColumnWidth() > 10) {
        vocableEditorView->setColumnWidth(3, m_vocableListModel->lessonColumnWidth());
    }
    connect(m_vocableListModel, SIGNAL(vocableChanged()), this, SLOT(refreshBoxFilter()));
    refreshBoxFilter();
}

void VocableList::refreshBoxFilter()
{
    QStringList boxes = m_vocableListModel->boxes();
    int i = 0;
    foreach (QString box, boxes) {
        if (boxSelectionCombo->count()-1 <= i) {
            boxSelectionCombo->addItem(box);
        } else {
            boxSelectionCombo->setItemText(i+1, box);
        }
        i++;
    }
    while (boxSelectionCombo->count()-1 > i) {
        boxSelectionCombo->removeItem(i+1);
    }
}

int VocableList::columnWidth(int num)
{
    return vocableEditorView->columnWidth(num);
}
QList<int> VocableList::selectedRows()
{
    QModelIndexList selectedIndexes = vocableEditorView->selectionModel()->selection().indexes();
    QList<int> rows;
    foreach(QModelIndex index, selectedIndexes) {
        int row = m_filteredVocableListModel->mapToSource(index).row();
        if (!rows.contains(row)) {
            rows << row;
        }
    }
    return rows;
}

QList<Vocable*> VocableList::selectedVocables()
{
    QModelIndexList selectedIndexes = vocableEditorView->selectionModel()->selection().indexes();
    QList<Vocable*> vocables;
    foreach(QModelIndex index, selectedIndexes) {
        Vocable* voc = m_vocableListModel->vocable(m_filteredVocableListModel->mapToSource(index));
        if (!vocables.contains(voc)) {
            vocables << voc;
        }
    }
    return vocables;
}

void VocableList::selectAll()
{
    QModelIndex topLeft = m_filteredVocableListModel->index(0, 0);
    QModelIndex bottomRight = m_filteredVocableListModel->index(m_filteredVocableListModel->rowCount()-1,
            m_filteredVocableListModel->columnCount()-1);
    QItemSelection selection(topLeft, bottomRight);
    vocableEditorView->selectionModel()->select(selection, QItemSelectionModel::Select);
}

QItemSelection VocableList::selection() const {
    return vocableEditorView->selectionModel()->selection();
}

