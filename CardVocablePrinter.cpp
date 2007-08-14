//
// C++ Implementation: CardVocablePrinter
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "CardVocablePrinter.h"
#include <QPainter>
#include <QPrinter>
#include "VocableListModel.h"
#include <QDebug>

CardVocablePrinter::CardVocablePrinter(VocableListModel *model, QPrinter *printer)
    : AbstractVocablePrinter(model, printer)
{
}

int CardVocablePrinter::pageCount()
{
    QPainter painter(m_printer);
    int pages = 1;
    int yPos = 0;

    int rows = m_vocableListModel->rowCount() / 2;
    if (!m_vocableListModel->rowCount() % 2) rows++;

    for (int row=0; row<rows; row++) {
        int height = rowHeight(row, painter);
        yPos += height+padding*2;
        if(yPos > m_printer->height()) {
            pages++;
            yPos = height+padding*2;
        }
    }
    return pages;
}
int CardVocablePrinter::rowHeight(int row, QPainter& painter)
{
    int height = 100;
    for (int i=0; i<2; i++) {
        if (m_vocableListModel->rowCount() <= row*2 + i) break;
        Vocable* voc = m_vocableListModel->vocable(row*2 + i);
        QRect rect = painter.boundingRect( QRect(0, 0, m_printer->width()/4 - padding*4, 100), Qt::TextWordWrap | Qt::AlignTop, voc->native());
        height = qMax(height, rect.height());
        rect = painter.boundingRect( QRect(0, 0, m_printer->width()/4 - padding*4, 100), Qt::TextWordWrap | Qt::AlignTop, voc->foreign());
        height = qMax(height, rect.height());
    }
    return height;
}
void CardVocablePrinter::printPage(int index, QPainter &painter)
{
    int yPos = 0;
    int currentIndex = 0;

    int rows = m_vocableListModel->rowCount() / 2;
    if (!m_vocableListModel->rowCount() % 2) rows++;

    for (int row=0; row<rows; row++) {
        int height = rowHeight(row, painter);

        if (yPos+height+padding*2 > m_printer->height()) {
            currentIndex++;
            if (currentIndex > index) {
                return;
            }
            yPos = 0;
        }
        
        if (currentIndex == index) {
            for (int i=0; i<2; i++) {
                if (m_vocableListModel->rowCount() <= row*2 + i) break;
                Vocable* voc = m_vocableListModel->vocable(row*2 + i);
                painter.drawText(QRect(padding + i*(m_printer->width()/4), yPos+padding, m_printer->width()/4-padding*2, height), Qt::TextWordWrap | Qt::AlignHCenter | Qt::AlignVCenter, voc->native());
                painter.drawRect(QRect(i*(m_printer->width()/4), yPos, m_printer->width()/4, height+padding*2));
                
                int left = m_printer->width()/2;
                if (i == 0) left += (m_printer->width()/4);
                painter.drawText(QRect(padding + left, yPos+padding, m_printer->width()/4-padding*2, height), Qt::TextWordWrap | Qt::AlignHCenter | Qt::AlignVCenter, voc->foreign());
                painter.drawRect(QRect(left, yPos, m_printer->width()/4, height+padding*2));
            }
        }
        yPos += height+padding*2;
    }
}

