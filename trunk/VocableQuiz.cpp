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
#include "VocableListModel.h"
#include "Vocable.h"
#include "VocableEditor.h"
#include "command/CommandQuizAnswer.h"
#include <QDialog>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QUndoStack>
#include <QKeyEvent>

VocableQuiz::VocableQuiz(VocableListModel* model, QUndoStack* undoStack, QuizType type, QStringList lessons)
    : m_vocableListModel(model), m_QuizType(type), m_lessons(lessons), m_currentVocalbeUnlearned(false), m_undoStack(undoStack)
{
	m_currentVocable = 0;

	m_Dialog = new QDialog();
    m_Dialog->setAttribute(Qt::WA_DeleteOnClose);
	m_ui = new Ui::VocableQuiz();
	m_ui->setupUi(m_Dialog);
	m_ui->resultTextLabel->setText("");

	connect(m_ui->checkButton, SIGNAL(clicked()), this, SLOT(checkVocable()));
	connect(m_ui->nextButton, SIGNAL(clicked()), this, SLOT(nextVocable()));
    connect(m_ui->editButton, SIGNAL(clicked()), this, SLOT(editVocable()));
    connect(m_ui->correctButton, SIGNAL(clicked()), this, SLOT(correctVocable()));
    m_ui->foreignLineEdit->installEventFilter(this);

    m_waitTimer = new QTimer(m_Dialog);
    m_waitTimer->setInterval(1000);
    connect(m_waitTimer, SIGNAL(timeout()), this, SLOT(nextVocable()));

    m_Dialog->show();
    nextVocable();
}

VocableQuiz::~VocableQuiz()
{
    delete m_ui;
}

void VocableQuiz::editVocable()
{
    if (m_currentVocable) {
        
        VocableEditor::editVocable(m_vocableListModel, m_currentVocable, m_undoStack);

        m_Dialog->activateWindow(); //bring quiz-window to foreground, fixme: doesn't really work :(

        m_ui->nativeLabel->setText(m_currentVocable->native());

        if (m_currentVocalbeUnlearned) {
            //just display it
            m_ui->resultLabel->setText(m_currentVocable->foreign());
        } else {
            //re-check if answer is now correct
            check(false);
        }
    }
}

void VocableQuiz::nextVocable()
{
    if(m_currentVocable && m_currentVocalbeUnlearned) {
		//unlearned, was just displayed; move to ultra-shortterm-memory
        CommandQuizAnswer* quizAnswerCommand = new CommandQuizAnswer(m_currentVocable);
        quizAnswerCommand->setBox(1);
        m_undoStack->push(quizAnswerCommand);
    }

    m_currentVocable = m_vocableListModel->randomVocable(m_QuizType, m_lessons);

	if(!m_currentVocable) {
        m_ui->quizDialogStackedWidget->setCurrentWidget(m_ui->waitPage);
        QDateTime expiredDate = m_vocableListModel->nextExpiredVocable(m_QuizType, m_lessons);
        if (!expiredDate.isValid()) {
            m_Dialog->hide();
            QMessageBox::information(m_Dialog,
                        tr("Vocable Quiz"),
                        tr("No vocables match the specified filter."));
            return;
        }
        int secs = QDateTime::currentDateTime().secsTo(expiredDate);
        QTime waitTime(0, 0, 0);
        waitTime = waitTime.addSecs(secs);
        int days = QDateTime::currentDateTime().daysTo(expiredDate);
        if (days) {
            m_ui->waitLabel->setText(tr("%1 days %2").arg(days).arg(waitTime.toString()));
        } else {
            m_ui->waitLabel->setText(waitTime.toString());
        }
        m_waitTimer->start();
		return;
	} else {
        m_waitTimer->stop();
        m_ui->quizDialogStackedWidget->setCurrentWidget(m_ui->quizPage);
    }
	if(m_currentVocable->box()==0) {
		//unlearned, don't ask just display it
        m_ui->helpLabel->setText(tr("Try to memorize the displayed vocable."));
		m_ui->buttonsStack->setCurrentWidget(m_ui->nextPage);
		m_ui->nativeLabel->setText(m_currentVocable->native());
		m_ui->resultTextLabel->setText(tr(""));
		m_ui->resultLabel->setText(m_currentVocable->foreign());
        m_ui->correctButton->hide();
        m_currentVocalbeUnlearned = true;
        m_ui->nextButton->setFocus();
	} else {
        m_ui->helpLabel->setText("");
		m_ui->buttonsStack->setCurrentWidget(m_ui->checkPage);
		m_ui->nativeLabel->setText(m_currentVocable->native());
		m_ui->foreignLineEdit->setText("");
        m_currentVocalbeUnlearned = false;
        m_ui->foreignLineEdit->setFocus();
    }
}

void VocableQuiz::checkVocable()
{
    check(true);
	m_ui->buttonsStack->setCurrentWidget(m_ui->nextPage);
}

void VocableQuiz::correctVocable()
{
    CommandQuizAnswer* quizAnswerCommand = new CommandQuizAnswer(m_currentVocable);
    quizAnswerCommand->setBadCount(m_currentVocableLastBadCount); //reset it to old value, check increments it if answer is still wrong
    quizAnswerCommand->setBox(m_currentVocableLastBox+1);
    m_undoStack->push(quizAnswerCommand);

    nextVocable();
}

void VocableQuiz::check(bool isFirstCheck)
{
    CommandQuizAnswer* quizAnswerCommand = new CommandQuizAnswer(m_currentVocable);
    int correctBox;
    if (isFirstCheck) {
        m_currentVocableLastBox = m_currentVocable->box(); //needed for editVocable
        m_currentVocableLastBadCount = m_currentVocable->badCount(); //needed for editVocable
        quizAnswerCommand->incrementQueryCount();
        correctBox = m_currentVocable->box()+1;;
    } else {
        correctBox = m_currentVocableLastBox+1;
        quizAnswerCommand->setBadCount(m_currentVocableLastBadCount); //reset it to old value, check increments it if answer is still wrong
    }
    if( m_ui->foreignLineEdit->text() == m_currentVocable->foreign() )
    {
        quizAnswerCommand->setBox(correctBox);
        m_ui->resultTextLabel->setText(tr("correct :)"));
        m_ui->resultLabel->setText("<font color=\"green\">"+m_currentVocable->foreign()+"</font>");
        m_ui->correctButton->hide();
        m_ui->nextButton->setFocus();
    }
    else
    {
        quizAnswerCommand->setBox(0);
        quizAnswerCommand->incrementBadCount();
        m_ui->resultTextLabel->setText(tr("wrong :("));
        m_ui->resultLabel->setText("<font color=\"red\">"+m_currentVocable->foreign()+"</font>");
        m_ui->correctButton->show();
        m_ui->nextButton->setFocus();
    }
    m_undoStack->push(quizAnswerCommand);
}

bool VocableQuiz::eventFilter(QObject *target, QEvent *event)
{
    if (target == m_ui->foreignLineEdit) {
        if(event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            if (keyEvent->key() == Qt::Key_Enter) {
                checkVocable();
                return true;
            }
        }
    }
    return false;
}
