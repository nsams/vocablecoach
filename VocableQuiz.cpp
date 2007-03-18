//
// C++ Implementation: VocableQuiz
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "VocableQuiz.h"
#include <QDialog>
#include "VocableListModel.h"
#include "Vocable.h"
#include <QMessageBox>
#include <QTimer>

VocableQuiz::VocableQuiz(VocableListModel* model, QuizType type)
	: m_vocableListModel(model), m_QuizType(type)
{
	m_currentVocable = 0;

	m_Dialog = new QDialog();
	m_ui = new Ui::VocableQuiz();
	m_ui->setupUi(m_Dialog);
	m_ui->resultTextLabel->setText("");

	connect(m_ui->checkButton, SIGNAL(clicked()), this, SLOT(checkVocable()));
	connect(m_ui->nextButton, SIGNAL(clicked()), this, SLOT(nextVocable()));

	QTimer *timer = new QTimer(m_Dialog);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTimes()));
	timer->start(1000);

	m_ui->shortTermMemoryProgressBar->setMaximum(12*60);
	m_ui->ultraShortTermMemoryProgressBar->setMaximum(18);
	m_startTime = QDateTime::currentDateTime();

	updateTimes();

    m_Dialog->show();
    nextVocable();
}

VocableQuiz::~VocableQuiz()
{
}

void VocableQuiz::updateTimes() {
	int secs = m_startTime.secsTo(QDateTime::currentDateTime());
	m_ui->shortTermMemoryProgressBar->setValue(secs);
	m_ui->ultraShortTermMemoryProgressBar->setValue(secs);
}

void VocableQuiz::nextVocable()
{
	if(m_currentVocable && m_currentVocable->box()==0) {
		//unlearned, was just displayed; move to ultra-shortterm-memory
		m_currentVocable->setBox(1);
		m_currentVocable->setLastQuery(QDateTime::currentDateTime());
		m_vocableListModel->emitVocableChanged();
	}
	if(m_QuizType==Expired) {
		m_currentVocable = m_vocableListModel->randomVocable(false);
	} else {
		m_currentVocable = m_vocableListModel->randomVocable(true);
	}
	if(!m_currentVocable) {
		QMessageBox::information(0, "VocableQuiz", tr("No vocables found"));
		//delete m_Dialog;
		//delete this;
		m_Dialog->hide();
		delete this;
		return;
	}
	if(m_currentVocable->box()==0) {
		//unlearned, don't ask just display it
		m_ui->buttonsStack->setCurrentWidget(m_ui->nextPage);
		m_ui->nativeLabel->setText(m_currentVocable->native());
		m_ui->resultTextLabel->setText(tr(""));
		m_ui->resultLabel->setText(m_currentVocable->foreign());
	} else {
		m_ui->buttonsStack->setCurrentWidget(m_ui->checkPage);
		m_ui->nativeLabel->setText(m_currentVocable->native());
		m_ui->foreignLineEdit->setText("");
	}
}

void VocableQuiz::checkVocable()
{
    if( m_ui->foreignLineEdit->text() == m_currentVocable->foreign() )
	{
		m_currentVocable->setBox(m_currentVocable->box()+1);
		m_ui->resultTextLabel->setText(tr("correct :)"));
		m_ui->resultLabel->setText("<font color=\"green\">"+m_currentVocable->foreign()+"</font>");
	}
	else
	{
		m_currentVocable->setBox(0);
		m_ui->resultTextLabel->setText(tr("wrong :("));
		m_ui->resultLabel->setText("<font color=\"red\">"+m_currentVocable->foreign()+"</font>");
	}
	m_currentVocable->setLastQuery(QDateTime::currentDateTime());
	m_ui->buttonsStack->setCurrentWidget(m_ui->nextPage);

	m_vocableListModel->emitVocableChanged();
}
