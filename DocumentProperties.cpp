//
// C++ Implementation: DocumentProperties
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "DocumentProperties.h"
#include "VocableListModel.h"
#include <QUndoStack>
#include "command/CommandEditProperties.h"

DocumentProperties::DocumentProperties(VocableListModel* model, QUndoStack* undoStack, QWidget *parent)
    : QDialog(parent), m_model(model), m_undoStack(undoStack)
{
    setupUi(this);
    lineEditTitle->setText(m_model->title());
    lineEditAuthors->setText(m_model->authors());
    lineEditNativeLanguage->setText(m_model->nativeLanguage());
    lineEditForeignLanguage->setText(m_model->foreignLanguage());
    
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
}

DocumentProperties::~DocumentProperties()
{
}

void DocumentProperties::ok()
{
    CommandEditProperties* editPropertiesCommand = new CommandEditProperties(m_model);

    editPropertiesCommand->setTitle(lineEditTitle->text());
    editPropertiesCommand->setAuthors(lineEditAuthors->toPlainText());
    editPropertiesCommand->setForeignLanguage(lineEditForeignLanguage->text());
    editPropertiesCommand->setNativeLanguage(lineEditNativeLanguage->text());

    m_undoStack->push(editPropertiesCommand);

    accept();
}
