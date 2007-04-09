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

#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>
#include <QPrinter>
#include <QList>
#include <QStringList>
#include "ui/ui_PreviewDialog.h"
#include "PreviewLabel.h"

class QTreeWidgetItem;
class AbstractVocablePrinter;
class VocableListModel;

class PreviewDialog : public QDialog, private Ui::PreviewDialog
{
    Q_OBJECT

public:
    PreviewDialog(VocableListModel *model, QPrinter &printer, QWidget *parent);
    ~PreviewDialog();
    enum { SmallPreviewLength = 200, LargePreviewLength = 400 };

    bool isSelected(int index);

protected:
    void resizeEvent(QResizeEvent *);

public slots:
    void accept();
    void addPage();
    void on_pageList_currentItemChanged();
    void on_paperSizeCombo_activated(int index);
    void on_paperOrientationCombo_activated(int index);
    void on_typeCombo_activated(int index);
    void reject();

private:
    void paintItem(QTreeWidgetItem *item, int index);
    void paintPreview(QPixmap &pixmap, int index);
    void setupComboBoxes();

    PreviewLabel *previewLabel;
    QPrinter &printer;
    bool canceled;
    int currentPage;
    int pageCount;
    
    VocableListModel *m_vocableListModel;
    AbstractVocablePrinter *vocablePrinter;
    void startAddPages();
};

#endif
