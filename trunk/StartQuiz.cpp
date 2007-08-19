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
#include <QMessageBox>

StartQuiz::StartQuiz(QWidget *parent, VocableListModel* model)
    : QDialog(parent)
{
    setupUi(this);

    foreach (QString lesson, model->lessons().values()) {
        QListWidgetItem* i = new QListWidgetItem(lesson, lessonsList);
        i->setSelected(true);
    }

    directionNativeForeignRadioButton->setText(QString("%1 » %2")
                .arg(model->nativeLanguage())
                .arg(model->foreignLanguage()));
    directionForeignNativeRadioButton->setText(QString("%1 » %2")
            .arg(model->foreignLanguage())
            .arg(model->nativeLanguage()));

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(tryAccept()));
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

void StartQuiz::tryAccept()
{
    if (!lessonsList->selectedItems().count()) {
        QMessageBox::information(this, tr("VocableCoach"),
                        tr("Please select at least one lesson."));
    } else {
        accept();
    }
}
