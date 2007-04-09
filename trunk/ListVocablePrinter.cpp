//
// C++ Implementation: ListVocablePrinter
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ListVocablePrinter.h"
#include <QPainter>
#include <QPrinter>
#include "VocableListModel.h"
ListVocablePrinter::ListVocablePrinter(VocableListModel *model, QPrinter *printer)
    : AbstractVocablePrinter(model, printer)
{
}

void ListVocablePrinter::printPage(int index, QPainter &painter)
{
    int yPos = 0;
    int currentIndex = 0;
    for(int i=0;i<m_vocableListModel->rowCount();i++)
    {
        Vocable* voc = m_vocableListModel->vocable(i);
        QRect rectNative = painter.boundingRect( QRect(0, yPos, m_printer->width()/2 - padding*2, 100), Qt::TextWordWrap | Qt::AlignTop, voc->native());
        QRect rectForeign = painter.boundingRect( QRect(0, yPos, m_printer->width()/2 - padding*2, 100), Qt::TextWordWrap | Qt::AlignTop, voc->foreign());
        int height = qMax(rectNative.height(), rectForeign.height());
        
        if (yPos+height+padding*2 > m_printer->height()) {
            currentIndex++;
            if (currentIndex > index) {
                return;
            }
            yPos = 0;
        }
        
        if (currentIndex == index) {
            painter.drawText(QRect(padding, yPos+padding, m_printer->width()/2-padding*2, height), Qt::TextWordWrap | Qt::AlignTop, voc->native());
            painter.drawRect(QRect(0, yPos, m_printer->width()/2, height+padding*2));
    
            painter.drawText(QRect(padding*2 + m_printer->width()/2, yPos+padding, m_printer->width()/2-padding*2, height), Qt::TextWordWrap | Qt::AlignTop, voc->foreign());
            painter.drawRect(QRect(m_printer->width()/2, yPos, m_printer->width()/2, height+padding*2));
        }

        yPos += height+padding*2;
    }
}

int ListVocablePrinter::pageCount()
{
    QPainter painter(m_printer);
    int pages = 1;
    int yPos = 0;
    for (int i=0; i<m_vocableListModel->rowCount(); i++) {
        Vocable* voc = m_vocableListModel->vocable(i);
        QRect rectNative = painter.boundingRect( QRect(0, yPos, m_printer->width()/2 - padding*2, 100), Qt::TextWordWrap | Qt::AlignTop, voc->native());
        QRect rectForeign = painter.boundingRect( QRect(0, yPos, m_printer->width()/2 - padding*2, 100), Qt::TextWordWrap | Qt::AlignTop, voc->foreign());
        int height = qMax(rectNative.height(), rectForeign.height());
        yPos += height+padding*2;
        
        if(yPos > m_printer->height()) {
            pages++;
            yPos = height+padding*2;
        }
    }
    return pages;
}
