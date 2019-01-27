#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <Qsci/qsciscintilla.h>
#include "editor.h"
#include "fileview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();

private:

    void createActions();
    void createMenus();
    void createStatusBar();

    void readSettings();
    void writeSettings();

    codeEditor *textEdit;
    QString curFile;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    fileViewer *fileManger;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
};

#endif // MAINWINDOW_H
