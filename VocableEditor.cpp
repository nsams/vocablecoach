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

VocableEditor::VocableEditor(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    httpLeo = new QHttp;
    httpLeo->setHost("pda.leo.org");  
    connect(httpLeo, SIGNAL(done(bool)), this, SLOT(readLeoData(bool)));
    
    connect(nativeTextEdit, SIGNAL(textChanged()), this, SLOT(nativeTextChanged()));
    connect(foreignTextEdit, SIGNAL(textChanged()), this, SLOT(foreignTextChanged()));

    nativeTextEdit->installEventFilter(this);
    foreignTextEdit->installEventFilter(this);

    startTranslationTimer = new QTimer;
    startTranslationTimer->setInterval(1000);
    startTranslationTimer->stop();
    connect(startTranslationTimer, SIGNAL(timeout()), this,  SLOT(translateLeo()));
}

VocableEditor::~VocableEditor()
{
    delete httpLeo;
    delete startTranslationTimer;
}

VocableEditor* VocableEditor::m_editor = 0;

VocableEditor* VocableEditor::getEditor()
{
	if(m_editor==0) {
		m_editor = new VocableEditor;
	}
	return m_editor;
}


void VocableEditor::addVocable(VocableListModel* model, QUndoStack* undoStack)
{
	VocableEditor* editor = getEditor();
    editor->nativeLabel->setText(model->nativeLanguage());
    editor->foreignLabel->setText(model->foreignLanguage());

    editor->boxLabel->setVisible(false);
	editor->boxLabelLeft->setVisible(false);
	editor->lastQueryLabel->setVisible(false);
	editor->lastQueryLabelLeft->setVisible(false);
	for(;;) {
        QString lastLesson = editor->lessonComboBox->currentText();
        editor->lessonComboBox->clear();
        QStringList lessons = model->lessons().values();
        editor->lessonComboBox->insertItems(0, lessons);
        editor->lessonComboBox->setCurrentIndex(lessons.indexOf(lastLesson));
		editor->nativeTextEdit->setPlainText("");
		editor->foreignTextEdit->setPlainText("");

		editor->nativeTextEdit->setFocus();
	
		if(editor->exec()==QDialog::Rejected) return;
		
        Vocable* vocable = model->createVocable();
	
		vocable->setNative(editor->nativeTextEdit->toPlainText());
		vocable->setForeign(editor->foreignTextEdit->toPlainText());
        vocable->setLesson(editor->lessonComboBox->currentText());

        CommandAdd* addCommand = new CommandAdd(model, vocable);
        undoStack->push(addCommand);
	}
}

int VocableEditor::editVocable(VocableListModel* model, Vocable* vocable, QUndoStack* undoStack)
{
	VocableEditor* editor = getEditor();
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
	editor->boxLabel->setVisible(true);
	editor->boxLabelLeft->setVisible(true);
	editor->lastQueryLabel->setVisible(true);
	editor->lastQueryLabelLeft->setVisible(true);
	
	int ret = editor->exec();
	if(ret==QDialog::Rejected) return ret;
	
    CommandEdit* editCommand = new CommandEdit(vocable);
    editCommand->setNative(editor->nativeTextEdit->toPlainText());
    editCommand->setForeign(editor->foreignTextEdit->toPlainText());
    editCommand->setLesson(editor->lessonComboBox->currentText());
    undoStack->push(editCommand);
	
	return ret;
}

bool VocableEditor::eventFilter(QObject *target, QEvent *event)
{
	if (target == nativeTextEdit || target == foreignTextEdit) {
		if(event->type() == QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            /*
			if (keyEvent->key() == Qt::Key_Tab) {
				focusNextChild();

				return true;
			} else*/
            if (!(keyEvent->modifiers() & Qt::ControlModifier)
                  &&  (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)) {
				accept();
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
    translateGroupBox->setTitle(tr("lookup vocable at leo.de..."));
    QUrl url("http://pda.leo.org/");
    url.addQueryItem("search", _translateText.simplified());
    httpLeo->get(url.path()+"?"+url.encodedQuery());
}

void VocableEditor::readLeoData(bool b)
{
    if (!b) {
        translateGroupBox->setTitle(tr("lookup vocable at leo.de"));
        QString Text(httpLeo->readAll());
        QRegExp rx("<input type=\"hidden\" name=\"lp\" value=\"ende\">[ \t]*<input type=\"hidden\" name=\"lang\" value=\"de\">([^\n]*)</table>");
        rx.setMinimal(true);
        rx.indexIn(Text);
        Text = rx.cap(1);
        Text = Text.remove(QRegExp("<td class=\"td1\" nowrap width=\"4%\"[^>]*><a href=\"/le\\?[0-9]+\"[^>]*><img border=\"0\" alt=\"[mp]\" width=14 height=16 src=\"/[mp].gif\"></a></td>"));
        Text = Text.remove("<td class=\"td1\" nowrap width=\"4%\" align=\"right\">&nbsp;</td>");
        Text = Text.remove("<td class=\"td1\" nowrap width=\"4%\">&nbsp;</td>");
        Text = Text.replace("<th colspan=2", "<th");
        Text = Text.replace("<TD ALIGN=CENTER COLSPAN=5>", "<TD ALIGN=CENTER COLSPAN=2>");
        Text = Text.replace("<TD COLSPAN=\"5\">", "<TD COLSPAN=\"2\">");
        Text = Text.remove(QRegExp("<A HREF=\"[^\"]*\">"));
        Text = Text.remove("</A>");
        if(Text.isEmpty()) Text = tr("no results...");
        translateTextBrowser->setHtml(Text);
    }
}
