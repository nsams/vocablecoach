//
// C++ Implementation: StatisticsWidget
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "StatisticsWidget.h"
#include "VocableListModel.h"
#include <QDebug>
#include <QLabel>
StatisticsWidget::StatisticsWidget(QWidget *parent)
    : QWidget(parent), m_vocableListModel(0)
{
    setupUi(this);
}


StatisticsWidget::~StatisticsWidget()
{
}

void StatisticsWidget::setModel(VocableListModel* model)
{
    m_vocableListModel = model;
    connect(m_vocableListModel, SIGNAL(vocableChanged()), this, SLOT(refresh()));
    refresh();
}
void StatisticsWidget::refresh()
{
    numVocablesLabel->setText(QString("%1").arg(m_vocableListModel->rowCount()));
    
    QStringList boxes = m_vocableListModel->boxes();
    int i = 0;
    foreach (QString box, boxes) {
        if (m_boxLabels.count() <= i) {
            QPair<QLabel*, QLabel*> pair;
            pair.first = new QLabel;
            pair.first->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            gridLayout->addWidget(pair.first, i+2, 0, 1, 1);
            pair.second = new QLabel;
            gridLayout->addWidget(pair.second, i+2, 1, 1, 1);
            m_boxLabels << pair;
        }
        QPair<QLabel*, QLabel*> pair = m_boxLabels[i];
        pair.first->setText(box + ":");
        pair.second->setText(QString("%1/%2")
                .arg(m_vocableListModel->inBoxCount(i, Vocable::NativeToForeign))
                .arg(m_vocableListModel->inBoxCount(i, Vocable::ForeignToNative)));
        i++;
    }
    while (m_boxLabels.count() > i) {
        QPair<QLabel*, QLabel*> pair = m_boxLabels.takeAt(i);
        delete pair.first;
        delete pair.second;
    }
}
