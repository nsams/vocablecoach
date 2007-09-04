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
#include "dictionary/DictionaryFactory.h"
#include <QDebug>
#include "dictionary/DictionaryAbstract.h"

DocumentProperties::DocumentProperties(VocableListModel* model, QUndoStack* undoStack, QWidget *parent)
    : QDialog(parent), m_model(model), m_undoStack(undoStack), m_nativeDictionary(0), m_foreignDictionary(0)
{
    setupUi(this);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(_ok()));

    connect(comboBoxNativeDictionary, SIGNAL(currentIndexChanged(int)), this, SLOT(_nativeDictionaryChanged(int)));
    connect(comboBoxForeignDictionary, SIGNAL(currentIndexChanged(int)), this, SLOT(_foreignDictionaryChanged(int)));
    connect(nativeDictionarySettings, SIGNAL(clicked()), this, SLOT(_openNativeDictionarySettings()));
    connect(foreignDictionarySettings, SIGNAL(clicked()), this, SLOT(_openForeignDictionarySettings()));
    connect(this, SIGNAL(rejected()), this, SLOT(_onRejected()));


    lineEditTitle->setText(m_model->title());
    lineEditAuthors->setText(m_model->authors());
    lineEditNativeLanguage->setText(m_model->nativeLanguage());
    lineEditForeignLanguage->setText(m_model->foreignLanguage());

    QListIterator<DictionaryName> it(DictionaryFactory::dictionaries());
    while (it.hasNext()) {
        DictionaryName d = it.next();
        comboBoxNativeDictionary->addItem(d.displayText, d.name);
        comboBoxForeignDictionary->addItem(d.displayText, d.name);
    }

    int i = comboBoxNativeDictionary->findData(m_model->nativeLanguageDict());
    if (i == -1) i = 0;
    comboBoxNativeDictionary->setCurrentIndex(i);

    i = comboBoxForeignDictionary->findData(m_model->foreignLanguageDict());
    if (i == -1) i = 0;
    comboBoxForeignDictionary->setCurrentIndex(i);

    m_editPropertiesCommand = new CommandEditProperties(m_model);
    m_editPropertiesCommand->setNativeLanguageSettings(m_model->nativeLanguageSettings());
    m_editPropertiesCommand->setForeignLanguageSettings(m_model->foreignLanguageSettings());
}

DocumentProperties::~DocumentProperties()
{
}

void DocumentProperties::_openNativeDictionarySettings()
{
    if (m_nativeDictionary) {
        m_editPropertiesCommand->setNativeLanguageSettings(m_nativeDictionary->editSettings(this));
    }
}

void DocumentProperties::_openForeignDictionarySettings()
{
    if (m_foreignDictionary) {
        m_editPropertiesCommand->setForeignLanguageSettings(m_foreignDictionary->editSettings(this));
    }
}

void DocumentProperties::_ok()
{
    m_editPropertiesCommand->setTitle(lineEditTitle->text());
    m_editPropertiesCommand->setAuthors(lineEditAuthors->toPlainText());
    m_editPropertiesCommand->setForeignLanguage(lineEditForeignLanguage->text());
    m_editPropertiesCommand->setNativeLanguage(lineEditNativeLanguage->text());
    m_editPropertiesCommand->setNativeLanguageDict(comboBoxNativeDictionary->itemData(comboBoxNativeDictionary->currentIndex()).toString());
    m_editPropertiesCommand->setForeignLanguageDict(comboBoxForeignDictionary->itemData(comboBoxForeignDictionary->currentIndex()).toString());
    

    m_undoStack->push(m_editPropertiesCommand);

    accept();
}
void DocumentProperties::_onRejected()
{
    delete m_editPropertiesCommand;
}

void DocumentProperties::_nativeDictionaryChanged(int index)
{
    delete m_nativeDictionary;
    m_nativeDictionary = DictionaryFactory::dictionaryInstance(
            comboBoxNativeDictionary->itemData(index).toString(),
            m_model->nativeLanguageSettings(), this);
    if (m_nativeDictionary && m_nativeDictionary->hasSettings()) {
        nativeDictionarySettings->show();
    } else {
        nativeDictionarySettings->hide();
    }
}
void DocumentProperties::_foreignDictionaryChanged(int index)
{
    delete m_foreignDictionary;
    m_foreignDictionary = DictionaryFactory::dictionaryInstance(
            comboBoxForeignDictionary->itemData(index).toString(),
            m_model->foreignLanguageSettings(), this);
    if (m_foreignDictionary && m_foreignDictionary->hasSettings()) {
        foreignDictionarySettings->show();
    } else {
        foreignDictionarySettings->hide();
    }
}
