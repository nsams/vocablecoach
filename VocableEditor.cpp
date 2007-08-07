//
// C++ Implementation: VocableEditor
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "VocableEditor.h"
#include "Vocable.h"
#include "VocableListModel.h"
#include "command/CommandAdd.h"
#include "command/CommandEdit.h"
#include <QKeyEvent>
#include <QHttp>
#include <QDebug>
#include <QTimer>
#include <QUrl>
#include <QUndoStack>
#include "dictionary/DictionaryLeo.h"

VocableEditor::VocableEditor(VocableListModel* model, QUndoStack* undoStack, QWidget *parent)
    : QDialog(parent), m_undoStack(undoStack), m_vocableListModel(model)
{
    setupUi(this);

    connect(okAddNewButton, SIGNAL(clicked()), this, SLOT(addVocable()));

    m_dictionary = new DictionaryLeo(this);
    connect(m_dictionary, SIGNAL(done(bool)), this, SLOT(translationDone(bool)));

    connect(nativeTextEdit, SIGNAL(textChanged()), this, SLOT(nativeTextChanged()));
    connect(foreignTextEdit, SIGNAL(textChanged()), this, SLOT(foreignTextChanged()));

    nativeTextEdit->installEventFilter(this);
    foreignTextEdit->installEventFilter(this);
    lessonComboBox->installEventFilter(this);

    startTranslationTimer = new QTimer(this);
    startTranslationTimer->setInterval(1000);
    startTranslationTimer->stop();
    connect(startTranslationTimer, SIGNAL(timeout()), this,  SLOT(translateLeo()));
}

void VocableEditor::addVocable(VocableListModel* model, QUndoStack* undoStack)
{
    VocableEditor* editor = new VocableEditor(model, undoStack);
    editor->nativeLabel->setText(model->nativeLanguage());
    editor->foreignLabel->setText(model->foreignLanguage());

    editor->okButton->setVisible(false);
    editor->okAddNewButton->setVisible(true);

    editor->boxLabel->setVisible(false);
	editor->boxLabelLeft->setVisible(false);
	editor->lastQueryLabel->setVisible(false);
	editor->lastQueryLabelLeft->setVisible(false);
    editor->queryCountLabel->setVisible(false);
    editor->queryCountLabelLeft->setVisible(false);
    editor->badCountLabel->setVisible(false);
    editor->badCountLabelLeft->setVisible(false);

    QString lastLesson = editor->lessonComboBox->currentText();
    editor->lessonComboBox->clear();
    QStringList lessons = model->lessons().values();
    editor->lessonComboBox->insertItems(0, lessons);
    editor->lessonComboBox->setCurrentIndex(lessons.indexOf(lastLesson));
    editor->nativeTextEdit->setPlainText("");
    editor->foreignTextEdit->setPlainText("");

    editor->nativeTextEdit->setFocus();
	
    editor->exec();

    delete editor;
}

int VocableEditor::editVocable(VocableListModel* model, Vocable* vocable, QUndoStack* undoStack)
{
    VocableEditor* editor = new VocableEditor(model, undoStack);
    editor->nativeLabel->setText(model->nativeLanguage());
    editor->foreignLabel->setText(model->foreignLanguage());

    QStringList lessons = model->lessons().values();
    editor->lessonComboBox->clear();
    editor->lessonComboBox->insertItems(0, lessons);

    editor->nativeTextEdit->setPlainText(vocable->native());
	editor->foreignTextEdit->setPlainText(vocable->foreign());
    editor->lessonComboBox->setCurrentIndex(lessons.indexOf(vocable->lesson()));
	editor->boxLabel->setNum(vocable->box());
	editor->lastQueryLabel->setText(vocable->lastQuery().toString(Qt::LocaleDate));
    editor->queryCountLabel->setText(QString("%1").arg(vocable->queryCount()));
    editor->badCountLabel->setText(QString("%1").arg(vocable->badCount()));

    editor->okButton->setVisible(true);
    editor->okAddNewButton->setVisible(false);
    editor->boxLabel->setVisible(true);
	editor->boxLabelLeft->setVisible(true);
	editor->lastQueryLabel->setVisible(true);
	editor->lastQueryLabelLeft->setVisible(true);
    editor->queryCountLabel->setVisible(true);
    editor->queryCountLabelLeft->setVisible(true);
    editor->badCountLabel->setVisible(true);
    editor->badCountLabelLeft->setVisible(true);
	
	int ret = editor->exec();
	if (ret != QDialog::Rejected) {
        CommandEdit* editCommand = new CommandEdit(vocable);
        editCommand->setNative(editor->nativeTextEdit->toPlainText());
        editCommand->setForeign(editor->foreignTextEdit->toPlainText());
        editCommand->setLesson(editor->lessonComboBox->currentText());
        undoStack->push(editCommand);
    }

	delete editor;
	return ret;
}

bool VocableEditor::eventFilter(QObject *target, QEvent *event)
{
    if (target == nativeTextEdit || target == foreignTextEdit || target == lessonComboBox) {
		if(event->type() == QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            /*
			if (keyEvent->key() == Qt::Key_Tab) {
				focusNextChild();

				return true;
			} else*/
            if (!(keyEvent->modifiers() & Qt::ControlModifier)
                  &&  (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)) {
                if(okAddNewButton->isVisible()) {
                    okAddNewButton->click();
                } else {
                    okButton->click();
                }
				return true;
			}
		}
	}
	return QDialog::eventFilter(target, event);
}

void VocableEditor::nativeTextChanged() {
    _translateText = nativeTextEdit->toPlainText();
    startTranslationTimer->start();
}
void VocableEditor::foreignTextChanged() {
    _translateText = foreignTextEdit->toPlainText();
    startTranslationTimer->start();
}


void VocableEditor::translateLeo()
{
    startTranslationTimer->stop();
    if (_translateText.simplified()=="") return;
    translateGroupBox->setTitle(tr("looking up vocable..."));
    m_dictionary->lookupWord(_translateText.simplified());
}

void VocableEditor::translationDone(bool error)
{
    if (!error) {
        QList<QPair<QString, QString> > results = m_dictionary->results();
        QString Text("");
        QPair<QString, QString> result;
        translateTreeWidget->clear();
        foreach(result, results) {
            QStringList items;
            items << result.first;
            items << result.second;
            new QTreeWidgetItem(translateTreeWidget, items);
        }
        translateGroupBox->setTitle(tr("looking up vocable"));
    } else {
        //Todo: show error
    }
}

void VocableEditor::addVocable()
{
    Vocable* vocable = m_vocableListModel->createVocable();
	
    vocable->setNative(nativeTextEdit->toPlainText());
    vocable->setForeign(foreignTextEdit->toPlainText());
    vocable->setLesson(lessonComboBox->currentText());

    CommandAdd* addCommand = new CommandAdd(m_vocableListModel, vocable);
    m_undoStack->push(addCommand);
    
    nativeTextEdit->clear();
    foreignTextEdit->clear();
    nativeTextEdit->setFocus();
}
