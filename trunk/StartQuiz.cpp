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
    directionNativeForeignRadioButton->setText(QString("%1 » %2")
                .arg(model->nativeLanguage())
                .arg(model->foreignLanguage()));
    directionForeignNativeRadioButton->setText(QString("%1 » %2")
            .arg(model->foreignLanguage())
            .arg(model->nativeLanguage()));
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

VocableQuiz::QuizType StartQuiz::quizType()
{
    if(onlyNewRadioButton->isChecked())
        return VocableQuiz::New;
    if(onlyExpiredRadioButton->isChecked())
        return VocableQuiz::Expired;

    return VocableQuiz::All;
}

Vocable::Direction StartQuiz::direction()
{
    if(directionNativeForeignRadioButton->isChecked())
        return Vocable::NativeToForeign;
    if(directionForeignNativeRadioButton->isChecked())
        return Vocable::ForeignToNative;

    return Vocable::Random;
}

