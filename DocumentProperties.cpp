//
// C++ Implementation: DocumentProperties
//
// Description: 
//
//
// Author: Niko Sams <ns@vivid-planet.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "DocumentProperties.h"
#include "VocableListModel.h"

DocumentProperties::DocumentProperties(VocableListModel* model, QWidget *parent)
    : QDialog(parent), m_model(model)
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
    m_model->setTitle(lineEditTitle->text());
    m_model->setAuthors(lineEditAuthors->toPlainText());
    m_model->setForeignLanguage(lineEditForeignLanguage->text());
    m_model->setNativeLanguage(lineEditNativeLanguage->text());
    accept();
}
