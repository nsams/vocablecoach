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
#include "VocableListWriter.h"
#include "VocableEditor.h"
#include "VocableQuiz.h"
#include "VocableListReader.h"
#include "VocableListModelFilter.h"
#include "StartQuiz.h"
#include "DocumentProperties.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    m_vocableListModel = new VocableListModel;
	m_filteredVocableListModel = new VocableListModelFilter();
	m_filteredVocableListModel->setSourceModel(m_vocableListModel);
	connect(m_vocableListModel, SIGNAL(vocableChanged()), m_filteredVocableListModel, SLOT(clear()));
	//m_filteredVocableListModel->setFilterKeyColumn(2);
	vocableEditorView->setModel(m_filteredVocableListModel);

    connect(actionImport, SIGNAL(triggered()), this, SLOT(import()));
    connect(actionPrint, SIGNAL(triggered()), this, SLOT(print()));
    connect(actionExport, SIGNAL(triggered()), this, SLOT(exportVocables()));
	connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    connect(actionQuiz, SIGNAL(triggered()), this, SLOT(startQuiz()));
    connect(actionProperties, SIGNAL(triggered()), this, SLOT(documentProperties()));
    
    connect(actionEditVocable, SIGNAL(triggered()), this, SLOT(editVocable()));
    connect(actionAddVocable, SIGNAL(triggered()), this, SLOT(addVocable()));
    connect(actionDeleteVocable, SIGNAL(triggered()), this, SLOT(deleteVocable()));
    
    connect(vocableEditorView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editVocable()));

    boxSelectionCombo->addItem("All");
    boxSelectionCombo->addItem("Unlearned");
    boxSelectionCombo->addItem("Ultra-Shortterm-Memory");
    boxSelectionCombo->addItem("Shortterm-Memory");
    boxSelectionCombo->addItem("Batch 1");
    boxSelectionCombo->addItem("Batch 2");
    boxSelectionCombo->addItem("Batch 3");
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
	if (maybeSave()) {
		m_vocableListModel->clearVocables();
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
				tr("Vocable-Files (*.kvtml)"));
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
			tr("save"),
			QString(),
			tr("Vocable-Files (*.kvtml)"));
	if (fileName.isEmpty())
		return false;

	return saveFile(fileName);
}


void MainWindow::loadFile(const QString &fileName)
{
	VocableListReader reader(fileName);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	reader.read(m_vocableListModel);
	QApplication::restoreOverrideCursor();
	setCurrentFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
	VocableListWriter writer(fileName);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	writer.write(m_vocableListModel);
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	return true;
}

void MainWindow::import()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("import"),
                QString(),
                "Pauker Files (*.xml *.xml.gz)");
	if(fileName=="") return;

	m_vocableListModel->importFile(fileName);
}
void MainWindow::print()
{
	QPrinter printer;
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName("/home/niko/test.pdf");
	QPrintDialog printDialog(&printer, this);
    
    printDialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    
    if (printDialog.exec() == QDialog::Accepted) {
        if(printDialog.printRange()==QAbstractPrintDialog::Selection) {
        }
        //QAbstractPrintDialog::AllPages
        //QAbstractPrintDialog::PageRange
    	QPainter painter(&printer);
    	static const int padding = 5;
    	int yPos = 0;
		for(int i=0;i<m_vocableListModel->rowCount();i++)
    	{
			Vocable* voc = m_vocableListModel->vocable(i);
			QRect rectNative = painter.boundingRect( QRect(0, yPos, printer.width()/2 - padding*2, 100), Qt::TextWordWrap | Qt::AlignTop, voc->native());
			QRect rectForeign = painter.boundingRect( QRect(0, yPos, printer.width()/2 - padding*2, 100), Qt::TextWordWrap | Qt::AlignTop, voc->foreign());
			int height = qMax(rectNative.height(), rectForeign.height());
			
			if(yPos+height > printer.height()) {
				printer.newPage();
				yPos = 0;
			}
			
			painter.drawText(QRect(padding, yPos+padding, printer.width()/2-padding*2, height), Qt::TextWordWrap | Qt::AlignTop, voc->native());
			painter.drawRect(QRect(0, yPos, printer.width()/2, height+padding*2));

			painter.drawText(QRect(padding*2 + printer.width()/2, yPos+padding, printer.width()/2-padding*2, height), Qt::TextWordWrap | Qt::AlignTop, voc->foreign());
			painter.drawRect(QRect(printer.width()/2, yPos, printer.width()/2, height+padding*2));

			yPos += height+padding*2;
    	}
    	painter.end();
    }
}
void MainWindow::exportVocables()
{
	QString fileName = QFileDialog::getSaveFileName(
			this,
			tr("export"),
			QString(),
			tr("CSV-Files (*.csv)"));
	if(fileName=="") return;

	QFile file(fileName);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::critical(0, tr("export"), tr("Can't open file"));
		return;
	}

	file.write(tr("Englisch;Deutsch;Box\n").toUtf8());

	for(int i=0;i<m_vocableListModel->rowCount();i++)
	{
		Vocable* voc = m_vocableListModel->vocable(i);
		QString out = "\"" + voc->foreign().replace("\n", "\\n").replace("\"", "\\\"") + "\";\"" + voc->native().replace("\n", "\\n").replace("\"", "\\\"") + "\";" + QString("%1").arg(voc->box()) + "\n";
		file.write(out.toUtf8());
	}
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
    new VocableQuiz(m_vocableListModel, startQuizDialog.quizType(), startQuizDialog.selectedLessions());
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

bool MainWindow::maybeSave()
{
	if (m_vocableListModel->isModified()) {
		int ret = QMessageBox::warning(this, tr("VocalbeCoach"),
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
		setCurrentFile("");
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about ( this, "About VocableCoach", "VocableCoach<br>&copy; Copyright by Niko Sams<br>Licence: GPLv2");
}
