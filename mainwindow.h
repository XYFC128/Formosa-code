#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <Qsci/qsciscintilla.h>
#include "fileview.h"
#include "tab.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    bool save();
    bool saveAs();
    void about();
    void copy();
    void cut();
    void newFile();
    void open();
    void paste();

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void readSettings();
    void writeSettings();

    QMenu       *editMenu;
    QMenu       *fileMenu;
    QMenu       *helpMenu;
    QString     curFile;
    Tab         *bookmark;
    fileViewer  *fileManger;

    QAction *aboutAct;
    QAction *copyAct;
    QAction *cutAct;
    QAction *exitAct;
    QAction *newAct;
    QAction *openAct;
    QAction *pasteAct;
    QAction *saveAct;
    QAction *saveAsAct;
};

#endif // MAINWINDOW_H
