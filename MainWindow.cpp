//
// C++ Implementation: MainWindow
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "MainWindow.h"
#include "VocableListModel.h"
#include "Vocable.h"
#include "VocableEditor.h"
#include "VocableQuiz.h"
#include "reader/ModelReaderKvtml.h"
#include "reader/ModelReaderPauker.h"
#include "reader/ModelReaderCsv.h"
#include "writer/ModelWriterKvtml.h"
#include "writer/ModelWriterCsv.h"
#include "VocableListModelFilter.h"
#include "StartQuiz.h"
#include "DocumentProperties.h"
#include "PreviewDialog.h"
#include "ListVocablePrinter.h"
#include "command/CommandDelete.h"
#include "command/CommandAdd.h"
#include <QFile>
#include <QByteArray>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTabBar>
#include <QPainter>
#include <QCloseEvent>
#include <QSettings>
#include <QProgressDialog>
#include <QClipboard>
#include <QUndoStack>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_vocableListModel(0)
{
    setupUi(this);
    
    m_undoStack = new QUndoStack(this);

    actionUndo->setEnabled(false);
    actionRedo->setEnabled(false);
    connect(actionUndo, SIGNAL(triggered()), m_undoStack, SLOT(undo()));
    connect(actionRedo, SIGNAL(triggered()), m_undoStack, SLOT(redo()));

    connect(m_undoStack, SIGNAL(canRedoChanged(bool)),
            actionRedo, SLOT(setEnabled(bool)));
    connect(m_undoStack, SIGNAL(canUndoChanged(bool)),
            actionUndo, SLOT(setEnabled(bool)));

    connect(m_undoStack, SIGNAL(cleanChanged(bool)),
            this, SLOT(cleanChanged(bool)));

    connect(menuEdit, SIGNAL(aboutToShow()),
            this, SLOT(itemMenuAboutToShow()));
    connect(menuEdit, SIGNAL(aboutToHide()),
            this, SLOT(itemMenuAboutToHide()));

    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    m_filteredVocableListModel = new VocableListModelFilter(this);
    vocableEditorView->setModel(m_filteredVocableListModel);

    vocableEditorView->setContextMenuPolicy(Qt::ActionsContextMenu);
    vocableEditorView->addAction(actionCut);
    vocableEditorView->addAction(actionCopy);
    vocableEditorView->addAction(actionPaste);
    QAction* separator = new QAction(QString(""), this);
    separator->setSeparator(true);
    vocableEditorView->addAction(separator);
    vocableEditorView->addAction(actionDeleteVocable);
    vocableEditorView->addAction(actionEditVocable);


    connect(actionImport, SIGNAL(triggered()), this, SLOT(import()));
    connect(actionPrint, SIGNAL(triggered()), this, SLOT(print()));
    connect(actionPrintPreview, SIGNAL(triggered()), this, SLOT(printPreview()));
    connect(actionExport, SIGNAL(triggered()), this, SLOT(exportVocables()));
	connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    connect(actionQuiz, SIGNAL(triggered()), this, SLOT(startQuiz()));
    connect(actionProperties, SIGNAL(triggered()), this, SLOT(documentProperties()));
    
    connect(actionCut, SIGNAL(triggered()), this, SLOT(cut()));
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(actionPaste, SIGNAL(triggered()), this, SLOT(paste()));
    
    connect(actionEditVocable, SIGNAL(triggered()), this, SLOT(editVocable()));
    connect(actionAddVocable, SIGNAL(triggered()), this, SLOT(addVocable()));
    connect(actionDeleteVocable, SIGNAL(triggered()), this, SLOT(deleteVocable()));
    
    connect(vocableEditorView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editVocable()));

    boxSelectionCombo->addItem(tr("All"));
    boxSelectionCombo->addItem(tr("Unlearned"));
    boxSelectionCombo->addItem(tr("Ultra-Shortterm-Memory"));
    boxSelectionCombo->addItem(tr("Shortterm-Memory"));
    boxSelectionCombo->addItem(tr("Box 1"));
    boxSelectionCombo->addItem(tr("Box 2"));
    boxSelectionCombo->addItem(tr("Box 3"));
    boxSelectionCombo->setCurrentIndex(0);
    connect(boxSelectionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(boxFilterChanged(int)));
    boxFilterChanged(0);
    
    connect(filterLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(textFilterChanged(const QString&)));
	
	readSettings();
}


void MainWindow::boxFilterChanged(int box)
{
    m_filteredVocableListModel->setBoxFilter(box-1);
}
void MainWindow::textFilterChanged(const QString& text)
{
    m_filteredVocableListModel->setTextFilter(text);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) {
		writeSettings();
		event->accept();
	} else {
		event->ignore();
	}
}

void MainWindow::newFile()
{
    if (!m_vocableListModel || maybeSave()) {
        VocableListModel* oldModel = m_vocableListModel;
        m_vocableListModel = new VocableListModel(this);
        m_vocableListModel->setNativeLanguage(tr("native"));
        m_vocableListModel->setForeignLanguage(tr("foreign"));
        m_filteredVocableListModel->setSourceModel(m_vocableListModel);
        delete oldModel;
        setCurrentFile("");
        m_undoStack->clear();
        cleanChanged(true);
	}
}

void MainWindow::open()
{
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(
				this,
				tr("open"),
				QString(),
                tr("All supported files (*.kvtml *.xml *.xml.gz *.csv *.txt);;KDE Vocabledocument (*.kvtml);;Pauker-Lektion (*.xml *.xml.gz);;CSV-Textdocument (*.csv *.txt)"));
		if (!fileName.isEmpty())
			loadFile(fileName);
	}
}

bool MainWindow::save()
{
	if (m_curFileName.isEmpty()) {
		return saveAs();
	} else {
		return saveFile(m_curFileName);
	}
}
bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(
			this,
            tr("Save Vocables"),
			QString(),
            tr("KDE Vocabledocument (*.kvtml);;CSV-Textdocument (*.csv *.txt)"));
	if (fileName.isEmpty())
		return false;

	bool ret = saveFile(fileName);
    if (ret) {
        setCurrentFile(fileName);
    }
    return ret;
}


void MainWindow::loadFile(const QString &fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QList<ModelReaderAbstract*> readers;
    readers << new ModelReaderKvtml(fileName);
    readers << new ModelReaderPauker(fileName);
    readers << new ModelReaderCsv(fileName);
    foreach (ModelReaderAbstract* reader, readers) {
        if (reader->isValidFile()) {
            VocableListModel* oldModel = m_vocableListModel;
            m_vocableListModel = new VocableListModel(this);
            reader->read(m_vocableListModel, 0);
            m_filteredVocableListModel->setSourceModel(m_vocableListModel);
            delete oldModel;
            setCurrentFile(fileName);
            m_undoStack->clear();
            cleanChanged(true);
            vocableEditorView->setColumnWidth(0, m_vocableListModel->nativeColumnWidth());
            vocableEditorView->setColumnWidth(1, m_vocableListModel->foreignColumnWidth());
            vocableEditorView->setColumnWidth(3, m_vocableListModel->lessonColumnWidth());
            QApplication::restoreOverrideCursor();
            qDeleteAll(readers);
            return;
        }
    }
    qDeleteAll(readers);
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(this, tr("VocableCoach"), tr("Can't open file: unknown Format"));

}

bool MainWindow::saveFile(const QString &fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ModelWriterAbstract* writer;
    if (fileName.right(6) == ".kvtml") {
        writer = new ModelWriterKvtml(fileName);
    } else if (fileName.right(4) == ".csv" || fileName.right(4) == ".txt") {
        writer = new ModelWriterCsv(fileName);
    }

    m_vocableListModel->setNativeColumnWidth(vocableEditorView->columnWidth(0));
    m_vocableListModel->setForeignColumnWidth(vocableEditorView->columnWidth(1));
    m_vocableListModel->setLessonColumnWidth(vocableEditorView->columnWidth(3));
    writer->write(m_vocableListModel);
    delete writer;
    cleanChanged(true);
	QApplication::restoreOverrideCursor();
	return true;
}

void MainWindow::import()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Import"),
                QString(),
                tr("All supported files (*.kvtml *.xml *.xml.gz *.csv *.txt);;KDE Vocabledocument (*.kvtml);;Pauker-Lektion (*.xml *.xml.gz);;CSV-Textdocument (*.csv *.txt)"));
	if(fileName=="") return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ModelReaderAbstract* reader;
    reader = new ModelReaderKvtml(fileName);
    if(!reader->isValidFile()) {
        reader = new ModelReaderPauker(fileName);
        if(!reader->isValidFile()) {
            QMessageBox::critical(this, tr("VocableCoach"), tr("Can't open file: unknown Format"));
            return;
        }
    }
    reader->read(m_vocableListModel, m_undoStack);
    delete reader;
    QApplication::restoreOverrideCursor();
}

bool MainWindow::setupPrinter(QPrinter &printer)
{
    QPrintDialog dialog(&printer, this);
//     dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if (dialog.exec() == QDialog::Accepted)
        return true;
    else
        return false;
}

void MainWindow::print()
{
     if (m_vocableListModel->rowCount() == 0)
          return;

    QPrinter printer(QPrinter::HighResolution);

    if (!setupPrinter(printer))
        return;
    
    ListVocablePrinter vocablePrinter(m_vocableListModel, &printer);
    
    int pageCount = vocablePrinter.pageCount();

    int from = printer.fromPage();
    int to = printer.toPage();
    if (from <= 0 && to <= 0) {
        from = 1;
        to = pageCount;
    }

    QProgressDialog progress(tr("Printing vocables..."), tr("&Cancel"),
                             from - 1, to, this);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowTitle(tr("Printing"));

    QPainter painter;
    painter.begin(&printer);
    bool firstPage = true;

    for (int index = from - 1; index < to; ++index) {

        if (!firstPage)
            printer.newPage();

        qApp->processEvents();
        if (progress.wasCanceled())
             break;

        vocablePrinter.printPage(index, painter);
        progress.setValue(index + 1);
        firstPage = false;
    }

    painter.end();
}

void MainWindow::printPreview()
{
    QPrinter printer;
    PreviewDialog preview(m_vocableListModel, printer, this);
    preview.exec();
}

void MainWindow::exportVocables()
{
	QString fileName = QFileDialog::getSaveFileName(
			this,
			tr("Export Vocables"),
			QString(),
            tr("KDE Vocabledocument (*.kvtml);;CSV-Textdocument (*.csv *.txt)"));
	if(fileName=="") return;
    
    saveFile(fileName);
}


void MainWindow::addVocable()
{
    VocableEditor::addVocable(m_vocableListModel, m_undoStack);
}

void MainWindow::editVocable()
{
	QModelIndex currentIndex = vocableEditorView->selectionModel()->currentIndex();
	if(!currentIndex.isValid()) return;

    Vocable* vocable = m_vocableListModel->vocable(m_filteredVocableListModel->mapToSource(currentIndex));
    VocableEditor::editVocable(m_vocableListModel, vocable, m_undoStack);
}
void MainWindow::cut()
{
    copy();
    QModelIndexList selectedIndexes = vocableEditorView->selectionModel()->selection().indexes();
    QList<int> rows;
    foreach(QModelIndex index, selectedIndexes) {
        int row = m_filteredVocableListModel->mapToSource(index).row();
        if (!rows.contains(row)) {
            rows << row;
        }
    }
    CommandDelete* delCommand = new CommandDelete(m_vocableListModel, rows);
    if (rows.count() > 1) {
        delCommand->setText(tr("Cut Vocables"));
    } else {
        delCommand->setText(tr("Cut Vocable"));
    }
    m_undoStack->push(delCommand);
}

void MainWindow::copy()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QStringList lines;

    QModelIndexList rows( vocableEditorView->selectionModel()->selectedRows() );
    foreach( const QModelIndex & index, rows ) {
        int row = m_filteredVocableListModel->mapToSource(index).row();
        lines << ModelWriterCsv::vocableCsvString(m_vocableListModel->vocable(row));
    }

    if (!lines.isEmpty()) {
        QApplication::clipboard()->setText(lines.join("\n"));
    }

    QApplication::restoreOverrideCursor();
}

void MainWindow::paste()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QModelIndex currentIndex = m_filteredVocableListModel->mapToSource(vocableEditorView->selectionModel()->currentIndex());
    int currentRow;
    if (!currentIndex.isValid()) {
        currentRow = m_vocableListModel->rowCount()-1;
    } else {
        currentRow = currentIndex.row();
    }
    QStringList lines = QApplication::clipboard()->text().split("\n");

    QUndoCommand* pasteCommand = new QUndoCommand();
    if (lines.count() > 1) {
        pasteCommand->setText(tr("Paste Vocables"));
    } else {
        pasteCommand->setText(tr("Paste Vocable"));
    }
    foreach (QString line, lines) {
        Vocable* voc = ModelReaderCsv::parseLine(m_vocableListModel, line.trimmed());
        new CommandAdd(m_vocableListModel, voc, currentRow+1, pasteCommand);
        currentRow++;
    }
    m_undoStack->push(pasteCommand);

    QApplication::restoreOverrideCursor();
}

void MainWindow::deleteVocable()
{
    QModelIndexList selectedIndexes = vocableEditorView->selectionModel()->selection().indexes();
    QList<int> rows;
    foreach(QModelIndex index, selectedIndexes) {
        int row = m_filteredVocableListModel->mapToSource(index).row();
        if (!rows.contains(row)) {
            rows << row;
        }
    }
    CommandDelete* delCommand = new CommandDelete(m_vocableListModel, rows);
    m_undoStack->push(delCommand);
}


void MainWindow::startQuiz()
{
    StartQuiz startQuizDialog(this, m_vocableListModel);
    if(startQuizDialog.exec()==QDialog::Rejected) return;
    new VocableQuiz(m_vocableListModel, m_undoStack, startQuizDialog.quizType(), startQuizDialog.selectedLessons());
    //fixme: leaks memory
}
void MainWindow::documentProperties()
{
    DocumentProperties prop(m_vocableListModel, m_undoStack, this);
    prop.exec();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	m_curFileName = fileName;
	setWindowModified(false);

	QString shownName;
	if (m_curFileName.isEmpty())
		shownName = "untitled.kvtml";
	else
		shownName = strippedName(m_curFileName);

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("VocableCoach")));
}
QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

bool MainWindow::maybeSave()
{
	if (!m_undoStack->isClean()) {
		int ret = QMessageBox::warning(this, tr("VocableCoach"),
									   tr("The document has been modified.\n"
											   "Do you want to save your changes?"),
									   QMessageBox::Yes | QMessageBox::Default,
									   QMessageBox::No,
									   QMessageBox::Cancel | QMessageBox::Escape);
		if (ret == QMessageBox::Yes)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}


void MainWindow::writeSettings()
{
	QSettings settings("NikoSoft", "VocableCoach");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("openFileName", m_curFileName);
}

void MainWindow::readSettings()
{
	QSettings settings("NikoSoft", "VocableCoach");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
	QString fileName = settings.value("openFileName", QString()).toString();
	if (!fileName.isEmpty())
		loadFile(fileName);
	else
        newFile();
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about ( this, "About VocableCoach", "VocableCoach<br>&copy; Copyright by Niko Sams<br>Licence: GPLv2");
}

void MainWindow::itemMenuAboutToHide()
{
    //deleteAction->setEnabled(true);
}

void MainWindow::itemMenuAboutToShow()
{
    actionUndo->setText(tr("Undo ") + m_undoStack->undoText());
    actionRedo->setText(tr("Redo ") + m_undoStack->redoText());
    //deleteAction->setEnabled(!diagramScene->selectedItems().isEmpty());
}

void MainWindow::cleanChanged(bool clean)
{
    actionSave->setEnabled(!clean);
    setWindowModified(!clean);
}
