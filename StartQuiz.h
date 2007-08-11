//
// C++ Interface: StartQuiz
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef STARTQUIZ_H
#define STARTQUIZ_H

#include <QDialog>
#include "ui/ui_StartQuiz.h"
#include "VocableQuiz.h" //for QuizType enum
class VocableListModel;

/**
	@author Niko Sams <niko.sams@gmail.com>
 */
class StartQuiz : public QDialog, private Ui::StartQuiz
{
    Q_OBJECT
public:
    StartQuiz(QWidget *parent, VocableListModel* model);
            
    ~StartQuiz();
    
    QStringList selectedLessons();
    VocableQuiz::QuizType quizType();
};

#endif
