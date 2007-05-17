//
// C++ Implementation: StartQuiz
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "StartQuiz.h"
#include "VocableListModel.h"
#include "VocableQuiz.h"

StartQuiz::StartQuiz(QWidget *parent, VocableListModel* model)
    : QDialog(parent)
{
    setupUi(this);
    lessonsList->addItems(model->lessons().values());
}


StartQuiz::~StartQuiz()
{
}


QStringList StartQuiz::selectedLessons()
{
    QStringList ret;
    foreach(QListWidgetItem* i, lessonsList->selectedItems()) {
        ret << i->text();
    }
    return ret;
}

QuizType StartQuiz::quizType()
{
    if(onlyNewRadioButton->isChecked())
        return New;
    if(onlyExpiredRadioButton->isChecked())
        return Expired;

    return All;
}
