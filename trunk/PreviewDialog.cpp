/****************************************************************************
**
** Copyright (C) 2006-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>
#include "PreviewDialog.h"
#include <QDebug>
#include "ListVocablePrinter.h"
#include "CardVocablePrinter.h"
#include "VocableListModel.h"

PreviewDialog::PreviewDialog(VocableListModel* model, QPrinter &printer, QWidget *parent)
    : QDialog(parent), printer(printer), m_vocableListModel(model)
{
    setupUi(this);

    vocablePrinter = new ListVocablePrinter(m_vocableListModel, &printer);
    
    pageList->setIconSize(QSize(SmallPreviewLength, SmallPreviewLength));
    pageList->header()->hide();
    previewLabel = new PreviewLabel;
    previewArea->setWidget(previewLabel);
    setupComboBoxes();

    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("&Print"));

    startAddPages();
}
PreviewDialog::~PreviewDialog()
{
    delete vocablePrinter;
}

void PreviewDialog::startAddPages()
{
    currentPage = 0;
    pageCount = vocablePrinter->pageCount();
    pageList->clear();
    QTimer::singleShot(0, this, SLOT(addPage()));
}

void PreviewDialog::setupComboBoxes()
{
    paperSizeCombo->addItem(tr("A0 (841 x 1189 mm)"), QPrinter::A0);
    paperSizeCombo->addItem(tr("A1 (594 x 841 mm)"), QPrinter::A1);
    paperSizeCombo->addItem(tr("A2 (420 x 594 mm)"), QPrinter::A2);
    paperSizeCombo->addItem(tr("A3 (297 x 420 mm)"), QPrinter::A3);
    paperSizeCombo->addItem(tr("A4 (210 x 297 mm, 8.26 x 11.7 inches)"), QPrinter::A4);
    paperSizeCombo->addItem(tr("A5 (148 x 210 mm)"), QPrinter::A5);
    paperSizeCombo->addItem(tr("A6 (105 x 148 mm)"), QPrinter::A6);
    paperSizeCombo->addItem(tr("A7 (74 x 105 mm)"), QPrinter::A7);
    paperSizeCombo->addItem(tr("A8 (52 x 74 mm)"), QPrinter::A8);
    paperSizeCombo->addItem(tr("A9 (37 x 52 mm)"), QPrinter::A9);
    paperSizeCombo->addItem(tr("B0 (1000 x 1414 mm)"), QPrinter::B0);
    paperSizeCombo->addItem(tr("B1 (707 x 1000 mm)"), QPrinter::B1);
    paperSizeCombo->addItem(tr("B2 (500 x 707 mm)"), QPrinter::B2);
    paperSizeCombo->addItem(tr("B3 (353 x 500 mm)"), QPrinter::B3);
    paperSizeCombo->addItem(tr("B4 (250 x 353 mm)"), QPrinter::B4);
    paperSizeCombo->addItem(tr("B5 (176 x 250 mm, 6.93 x 9.84 inches)"), QPrinter::B5);
    paperSizeCombo->addItem(tr("B6 (125 x 176 mm)"), QPrinter::B6);
    paperSizeCombo->addItem(tr("B7 (88 x 125 mm)"), QPrinter::B7);
    paperSizeCombo->addItem(tr("B8 (62 x 88 mm)"), QPrinter::B8);
    paperSizeCombo->addItem(tr("B9 (44 x 62 mm)"), QPrinter::B9);
    paperSizeCombo->addItem(tr("B10 (31 x 44 mm)"), QPrinter::B10);
    paperSizeCombo->addItem(tr("C5E (163 x 229 mm)"), QPrinter::C5E);
    paperSizeCombo->addItem(tr("DLE (110 x 220 mm)"), QPrinter::DLE);
    paperSizeCombo->addItem(tr("Executive (7.5 x 10 inches, 191 x 254 mm)"), QPrinter::Executive);
    paperSizeCombo->addItem(tr("Folio (210 x 330 mm)"), QPrinter::Folio);
    paperSizeCombo->addItem(tr("Ledger (432 x 279 mm)"), QPrinter::Ledger);
    paperSizeCombo->addItem(tr("Legal (8.5 x 14 inches, 216 x 356 mm)"), QPrinter::Legal);
    paperSizeCombo->addItem(tr("Letter (8.5 x 11 inches, 216 x 279 mm)"), QPrinter::Letter);
    paperSizeCombo->addItem(tr("Tabloid (279 x 432 mm)"), QPrinter::Tabloid);
    paperSizeCombo->addItem(tr("US Common #10 Envelope (105 x 241 mm)"), QPrinter::Comm10E);
    paperSizeCombo->setCurrentIndex(paperSizeCombo->findData(QPrinter::A4));

    paperOrientationCombo->addItem(tr("Portrait"), QPrinter::Portrait);
    paperOrientationCombo->addItem(tr("Landscape"), QPrinter::Landscape);

    typeCombo->addItem(tr("List"), "list");
    typeCombo->addItem(tr("Cards"), "card");
}

void PreviewDialog::addPage()
{
    if (currentPage >= pageCount)
        return;

    QTreeWidgetItem *item = new QTreeWidgetItem(pageList);
    item->setCheckState(0, Qt::Checked);

    paintItem(item, currentPage);
    if (pageList->indexOfTopLevelItem(pageList->currentItem()) < 0)
        pageList->setCurrentItem(pageList->topLevelItem(0));

    qApp->processEvents();
    ++currentPage;

    QTimer::singleShot(0, this, SLOT(addPage()));
}

void PreviewDialog::paintItem(QTreeWidgetItem *item, int index)
{
    QPixmap pixmap(SmallPreviewLength, SmallPreviewLength);
    paintPreview(pixmap, index);
    item->setIcon(0, QIcon(pixmap));
}

void PreviewDialog::paintPreview(QPixmap &pixmap, int index)
{
    qreal longestSide = qreal(qMax(printer.paperRect().width(),
                              printer.paperRect().height()));
    qreal width = pixmap.width() * printer.paperRect().width()/longestSide;
    qreal height = pixmap.height() * printer.paperRect().height()/longestSide;

    pixmap.fill(qRgb(224,224,224));
    QPainter painter;
    painter.begin(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate((pixmap.width() - width)/2,
                      (pixmap.height() - height)/2);
    painter.fillRect(QRectF(0, 0, width, height), QBrush(Qt::white));
    painter.scale(pixmap.width() / longestSide, pixmap.height() / longestSide);
    painter.translate(printer.pageRect().topLeft());
    vocablePrinter->printPage(index, painter);
    painter.end();
}

void PreviewDialog::accept()
{
    int markedPages = 0;
    int pageIndex;
    for (pageIndex = 0; pageIndex < pageList->topLevelItemCount(); ++pageIndex) {
        if (pageList->topLevelItem(pageIndex)->checkState(0) == Qt::Checked)
            ++markedPages;
    }

    // Print all pages that have yet to be previewed.
    markedPages += pageCount - pageIndex;

    printer.setPageSize(QPrinter::PageSize(
        paperSizeCombo->itemData(paperSizeCombo->currentIndex()).toInt()));
    printer.setOrientation(QPrinter::Orientation(
        paperOrientationCombo->itemData(
        paperOrientationCombo->currentIndex()).toInt()));

    QPrintDialog dialog(&printer, this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    progressBar->setMaximum(markedPages);
    progressBar->setEnabled(true);
    progressBar->setTextVisible(true);

    QPainter painter;
    painter.begin(&printer);

    canceled = false;
    int printed = 0;
    bool firstPage = true;
    for (pageIndex = 0; pageIndex < pageCount; ++pageIndex) {

        qApp->processEvents();

        if (canceled)
            break;

        if (pageIndex >= currentPage || isSelected(pageIndex)) {
            // Print all pages that are either marked or have yet to be
            // previewed.
            if (!firstPage)
                printer.newPage();

            vocablePrinter->printPage(pageIndex, painter);
            progressBar->setValue(++printed);
            firstPage = false;
        }
    }
    painter.end();

    progressBar->setTextVisible(false);
    progressBar->setEnabled(false);

    QDialog::accept();
}

bool PreviewDialog::isSelected(int index)
{
    if (index >= 0 && index < pageList->topLevelItemCount())
        return (pageList->topLevelItem(index)->checkState(0) == Qt::Checked);
    else
        return false;
}

void PreviewDialog::reject()
{
    canceled = true;
    QDialog::reject();
}

void PreviewDialog::resizeEvent(QResizeEvent *)
{
    int size = qMax(qMin(previewArea->width() - previewArea->verticalScrollBar()->width(),
                         previewArea->height() - previewArea->horizontalScrollBar()->height()),
                    int(LargePreviewLength));
    previewLabel->resize(size, size);
    on_pageList_currentItemChanged();
}

void PreviewDialog::on_pageList_currentItemChanged()
{
    if (pageList->indexOfTopLevelItem(pageList->currentItem()) < 0)
        return;

    QPixmap pixmap(previewLabel->size());
    paintPreview(pixmap, pageList->indexOfTopLevelItem(pageList->currentItem()));
    previewLabel->setPixmap(pixmap);
    previewLabel->update();
}

void PreviewDialog::on_paperSizeCombo_activated(int index)
{
    printer.setPageSize(QPrinter::PageSize(paperSizeCombo->itemData(index).toInt()));

    startAddPages();
}

void PreviewDialog::on_paperOrientationCombo_activated(int index)
{
    printer.setOrientation(QPrinter::Orientation(paperOrientationCombo->itemData(index).toInt()));

    startAddPages();
}

void PreviewDialog::on_typeCombo_activated(int index)
{
    delete vocablePrinter;
    if (typeCombo->itemData(index) == "list") {
        vocablePrinter = new ListVocablePrinter(m_vocableListModel, &printer);
    } else if (typeCombo->itemData(index) == "card") {
        vocablePrinter = new CardVocablePrinter(m_vocableListModel, &printer);
    } else {
        vocablePrinter = 0;
    }
    if (vocablePrinter) {
        startAddPages();
    }
//     on_pageList_currentItemChanged();
}
