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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_vocableListModel(0)
{
    setupUi(this);
    
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	m_filteredVocableListModel = new VocableListModelFilter();
    vocableEditorView->setModel(m_filteredVocableListModel);
    
    

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
    
    connect(m_vocableListModel, SIGNAL(vocableChanged()), this, SLOT(documentModified()));
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
        m_vocableListModel = new VocableListModel();
        m_vocableListModel->setNativeLanguage(tr("native"));
        m_vocableListModel->setForeignLanguage(tr("foreign"));
        m_filteredVocableListModel->setSourceModel(m_vocableListModel);
        connect(m_vocableListModel, SIGNAL(vocableChanged()), m_filteredVocableListModel, SLOT(clear()));
        delete oldModel;
        setCurrentFile("");
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
            m_vocableListModel = new VocableListModel();
            reader->read(m_vocableListModel);
            delete reader;
            m_filteredVocableListModel->setSourceModel(m_vocableListModel);
            connect(m_vocableListModel, SIGNAL(vocableChanged()), m_filteredVocableListModel, SLOT(clear()));
            delete oldModel;
            setCurrentFile(fileName);
            QApplication::restoreOverrideCursor();
            return;
        }
    }
    QMessageBox::critical(this, tr("VocableCoach"), tr("Can't open file: unknown Format"));

    QApplication::restoreOverrideCursor();
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
	writer->write(m_vocableListModel);
    delete writer;
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
    reader->read(m_vocableListModel);
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
	VocableEditor::addVocable(m_vocableListModel);
}

void MainWindow::editVocable()
{
	QModelIndex currentIndex = vocableEditorView->selectionModel()->currentIndex();
	if(!currentIndex.isValid()) return;

	Vocable* vocable = m_vocableListModel->vocable( currentIndex );
    VocableEditor::editVocable(m_vocableListModel, vocable);
}
void MainWindow::cut() {
}
void MainWindow::copy()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QStringList lines;
    
    QModelIndexList rows( vocableEditorView->selectionModel()->selectedRows() );
    foreach( const QModelIndex & index, rows ) {
        lines << ModelWriterCsv::vocableCsvString(m_vocableListModel->vocable(index));
    }

    if (!lines.isEmpty()) {
        QApplication::clipboard()->setText(lines.join("\n"));
    }
    QApplication::restoreOverrideCursor();
}
void MainWindow::paste() {
}
void MainWindow::deleteVocable()
{
	QItemSelection selection( vocableEditorView->selectionModel()->selection() );

	QList<int> rows;
	foreach( const QModelIndex & index, selection.indexes() ) {
		rows.append( index.row() );
	}

	qSort( rows );

	int prev = -1;
	for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
		int current = rows[i];
		if( current != prev ) {
			m_vocableListModel->removeRows( current, 1 );
			prev = current;
		}
	}
}


void MainWindow::startQuiz()
{
    StartQuiz startQuizDialog(this, m_vocableListModel);
    if(startQuizDialog.exec()==QDialog::Rejected) return;
    new VocableQuiz(m_vocableListModel, startQuizDialog.quizType(), startQuizDialog.selectedLessons());
}
void MainWindow::documentProperties()
{
    DocumentProperties prop(m_vocableListModel, this);
    prop.exec();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	m_curFileName = fileName;
	m_vocableListModel->setModified(false);
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

void MainWindow::documentModified() {
    setWindowModified(m_vocableListModel->isModified());
}

bool MainWindow::maybeSave()
{
	if (m_vocableListModel->isModified()) {
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
