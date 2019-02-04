#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>
#include <QUrl>
#include <QMimeData>
#include <QDragEnterEvent>

#include <QSysInfo>
#include "mainwindow.h"
#include "editor.h"
#include "fileview.h"
#include "tab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createStatusBar();
    readSettings();

    bookmark = new Tab;
    setCentralWidget(bookmark);
    setWindowTitle("Formosa-code");
    fileManger = new fileViewer(bookmark);
    addDockWidget(Qt::RightDockWidgetArea,fileManger);

}
MainWindow::~MainWindow()
{
    delete fileManger;
    delete bookmark;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(bookmark->closeAll())
        event->accept();
    return;
}
void MainWindow::newFile()
{
    bookmark->add("");
}
void MainWindow::open()
{
    if (bookmark->open()){
        statusBar()->showMessage(tr("檔案載入完成"), 2000);
    }
}
bool MainWindow::save()
{
    return bookmark->save();
}
bool MainWindow::saveAs()
{
    statusBar()->showMessage(tr("另存新檔"), 2000);
    bookmark->saveAs();
    return true;
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("關於Formosa code"),
                       tr("<b>Formosa code</b>是我在閒暇時間開發的一款文字編輯器<br>"
                          "支援基本的語法高亮和自動完成程式碼<br>"
                          "感謝您的使用<br>"));
}
void MainWindow::createActions()
{
    newAct = new QAction(tr("新增"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("新增檔案"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("開啟..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("開啟檔案"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("儲存"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("儲存檔案"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("另存新檔..."), this);
    saveAsAct->setStatusTip(tr("將檔案儲存成其他名稱"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("離開"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("關閉程式"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(tr("剪下"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("剪下所選內容"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("複製"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("複製所選內容"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("貼上"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("貼上剪貼簿內容"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    aboutAct = new QAction(tr("關於"), this);
    aboutAct->setStatusTip(tr("關於本程式"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));


}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("檔案"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("編輯"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);


    helpMenu = menuBar()->addMenu(tr("幫助"));
    helpMenu->addAction(aboutAct);
}
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("就緒"));
}
void MainWindow::readSettings()
{

    QSettings settings("Trolltech", "Application Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);

}
void MainWindow::writeSettings()
{
    QSettings settings("Trolltech", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
void MainWindow::cut(){
    bookmark->cut();
}
void MainWindow::copy(){
    bookmark->copy();
}
void MainWindow::paste(){
    bookmark->paste();
}
