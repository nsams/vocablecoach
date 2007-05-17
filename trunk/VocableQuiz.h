//
// C++ Interface: VocableQuiz
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOCABLEQUIZ_H
#define VOCABLEQUIZ_H
#include "ui/ui_VocableQuiz.h"
#include <QObject>
#include <QDateTime>

class QUndoStack;
class VocableListModel;
class QDialog;
class Vocable;
class CommandQuizAnswer;

enum QuizType
{
	All,
    New,
	Expired
};
/**
	@author Niko Sams <niko.sams@gmail.com>
*/
class VocableQuiz : public QObject {
	Q_OBJECT
public:
    VocableQuiz(VocableListModel* model, QUndoStack* undoStack, QuizType, QStringList lessons=QStringList());
    ~VocableQuiz();
	
	enum quizType {};

private:
	QDialog* m_Dialog;
	Ui::VocableQuiz* m_ui;
	VocableListModel* m_vocableListModel;
	QuizType m_QuizType;
	QDateTime m_startTime;
    QStringList m_lessons;
    QTimer* m_waitTimer;
    bool m_currentVocalbeUnlearned;
    int m_currentVocableLastBox;
    QUndoStack* m_undoStack;
    void check(int correctBox);
    bool eventFilter(QObject *target, QEvent *event);

private slots:
    void nextVocable();
    void checkVocable();
    void editVocable();
    void correctVocable();
protected:
    Vocable* m_currentVocable;
};

#endif