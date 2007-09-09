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
#include "dictionary/DictionaryFactory.h"
#include "dictionary/DictionaryAbstract.h"

VocableEditor::VocableEditor(VocableListModel* model, QUndoStack* undoStack, QWidget *parent)
    : QDialog(parent), m_undoStack(undoStack), m_vocableListModel(model)
{
    setupUi(this);

    connect(okAddNewButton, SIGNAL(clicked()), this, SLOT(addVocable()));

    connect(nativeTextEdit, SIGNAL(textChanged()), this, SLOT(nativeTextChanged()));
    connect(foreignTextEdit, SIGNAL(textChanged()), this, SLOT(foreignTextChanged()));

    nativeTextEdit->installEventFilter(this);
    foreignTextEdit->installEventFilter(this);
    lessonComboBox->installEventFilter(this);

    startTranslationTimerNative = new QTimer(this);
    startTranslationTimerNative->setInterval(1000);
    startTranslationTimerNative->stop();
    connect(startTranslationTimerNative, SIGNAL(timeout()), this,  SLOT(lookupWordNative()));

    startTranslationTimerForeign = new QTimer(this);
    startTranslationTimerForeign->setInterval(1000);
    startTranslationTimerForeign->stop();
    connect(startTranslationTimerForeign, SIGNAL(timeout()), this,  SLOT(lookupWordForeign()));

    m_dictionaryForeign = DictionaryFactory::dictionaryInstance(m_vocableListModel->foreignLanguageDict(), m_vocableListModel->foreignLanguageSettings(), this);
    if (m_dictionaryForeign) connect(m_dictionaryForeign, SIGNAL(done(bool)), this, SLOT(translationDone(bool)));
    m_dictionaryNative = DictionaryFactory::dictionaryInstance(m_vocableListModel->nativeLanguageDict(), m_vocableListModel->nativeLanguageSettings(), this);
    if (m_dictionaryNative) connect(m_dictionaryNative, SIGNAL(done(bool)), this, SLOT(translationDone(bool)));
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
	editor->boxLabel->setText(QString("%1/%2")
            .arg(vocable->box(Vocable::NativeToForeign))
            .arg(vocable->box(Vocable::ForeignToNative)));
    editor->lastQueryLabel->setText(vocable->lastQueryString());
    editor->queryCountLabel->setText(QString("%1/%2")
            .arg(vocable->queryCount(Vocable::NativeToForeign))
            .arg(vocable->queryCount(Vocable::ForeignToNative)));
    editor->badCountLabel->setText(QString("%1/%2")
            .arg(vocable->badCount(Vocable::NativeToForeign))
            .arg(vocable->badCount(Vocable::ForeignToNative)));

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
    _translateTextNative = nativeTextEdit->toPlainText();
    startTranslationTimerNative->start();
}
void VocableEditor::foreignTextChanged() {
    _translateTextForeign = foreignTextEdit->toPlainText();
    startTranslationTimerForeign->start();
}


void VocableEditor::lookupWordNative()
{
    startTranslationTimerNative->stop();
    if (_translateTextNative.simplified()=="") return;
    if (m_dictionaryNative) {
        translateGroupBox->setTitle(tr("dictionary..."));
        m_dictionaryNative->lookupWord(_translateTextNative.simplified());
    }
}

void VocableEditor::lookupWordForeign()
{
    startTranslationTimerForeign->stop();
    if (_translateTextForeign.simplified()=="") return;
    if (m_dictionaryForeign) {
        translateGroupBox->setTitle(tr("dictionary..."));
        m_dictionaryForeign->lookupWord(_translateTextForeign.simplified());
    }
}

void VocableEditor::translationDone(bool error)
{
    DictionaryAbstract* dictionary = qobject_cast<DictionaryAbstract*>(sender());
    if (!error && dictionary) {

        QPair<QString, QString> headerText = dictionary->headerText();
        translateTreeWidget->headerItem()->setText(0, headerText.first);
        translateTreeWidget->headerItem()->setText(1, headerText.second);
        translateTreeWidget->headerItem()->setHidden(headerText.first.isEmpty());

        QList<QPair<QString, QString> > results = dictionary->results();
        QString Text("");
        QPair<QString, QString> result;
        translateTreeWidget->clear();
        foreach(result, results) {
            QStringList items;
            items << result.first;
            items << result.second;
            new QTreeWidgetItem(translateTreeWidget, items);
        }
        translateGroupBox->setTitle(tr("dictionary"));
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
