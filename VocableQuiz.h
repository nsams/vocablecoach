//
// C++ Interface: VocableQuiz
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOCABLEQUIZ_H
#define VOCABLEQUIZ_H
#include "ui/ui_VocableQuiz.h"
#include <QObject>
#include <QDateTime>

class VocableListModel;
class QDialog;
class Vocable;

enum QuizType
{
	New,
	Expired
};
/**
	@author Niko Sams <ns@vivid-planet.com>
*/
class VocableQuiz : public QObject {
	Q_OBJECT
public:
	VocableQuiz(VocableListModel* model, QuizType);
    ~VocableQuiz();
	
	enum quizType {};

private:
	QDialog* m_Dialog;
	Ui::VocableQuiz* m_ui;
	VocableListModel* m_vocableListModel;
	QuizType m_QuizType;
	QDateTime m_startTime;
private slots:
    void nextVocable();
    void checkVocable();
	void updateTimes();
protected:
    Vocable* m_currentVocable;
};

#endif
