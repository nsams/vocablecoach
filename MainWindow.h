//
// C++ Interface: MainWindow
//
// Description: 
//
//
// Author: Niko Sams <niko.sams@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui/ui_MainWindow.h"
#include <QDialog>
class VocableListModel;
class VocableList;
class VocableListModelFilter;
class QUndoStack;
class Vocable;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private:
    VocableListModel* m_vocableListModel;
    VocableListModelFilter* m_filteredVocableListModel;
    QString m_curFileName;

    QUndoStack* m_undoStack;

    void setCurrentFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void loadFile(const QString &fileName);
    bool maybeSave();
    void writeSettings();
    void readSettings();
    bool setupPrinter(QPrinter &printer);
    QList<int> selectedRows();
    QList<Vocable*> selectedVocables();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void import();
    void print();
    void printPreview();
    void exportVocables();
    void newFile();
    void open();
    bool saveAs();
    bool save();
    void startQuiz();
    void documentProperties();

    void cut();
    void copy();
    void paste();

    void selectAll();
    void deleteVocable();
    void editVocable();
    void addVocable();

    void modifyLesson();
    void resetBox();

    void boxFilterChanged(int box);
    void textFilterChanged(const QString& text);
    void showAboutDialog();

    void editMenuAboutToShow();
    void selectionChanged();

    void cleanChanged(bool clean);
};

#endif
