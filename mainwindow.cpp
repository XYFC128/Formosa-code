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

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SetupKeyList();
    SetupEditor("txt");
    setCentralWidget(textEdit);


    createActions();
    createMenus();
    //createToolBars();
    createStatusBar();

    readSettings();

    connect(textEdit, SIGNAL(textChanged()),this, SLOT(documentWasModified()));

    setCurrentFile("");
}

MainWindow::~MainWindow()
{

}

bool MainWindow::SetupEditor(QString s){
    if(textEdit == nullptr) textEdit = new QsciScintilla;
    if(s == "cpp"||s == "h"){
        QsciLexerCPP *textLexer = new QsciLexerCPP;
        textEdit->setLexer(textLexer);
        QsciAPIs *apis = new QsciAPIs(textLexer);
        foreach(const QString &keyword,cppkeylist){
            apis->add(keyword);
        }
        apis->prepare();
    }
    else if (s == "py") {
        QsciLexerPython *textLexer = new QsciLexerPython;
        textEdit->setLexer(textLexer);
        QsciAPIs *apis = new QsciAPIs(textLexer);
        foreach(const QString &keyword,pykeylist){
            apis->add(keyword);
        }
        apis->prepare();
    }

    textEdit->setMarginType(0,QsciScintilla::NumberMargin);
    textEdit->setMarginLineNumbers(0,true);
    textEdit->setMarginWidth(0,31);

    textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
    textEdit->setAutoCompletionCaseSensitivity(true);
    textEdit->setAutoCompletionThreshold(2);
    textEdit->setFont(QFont("Courier New"));
    textEdit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);
    return true;
}
bool MainWindow::SetupKeyList(){
    cppkeylist<<"#include"<<"#define"<<"#ifndef"<<"#endef"<<"auto"<<"break"<<"case"<<"catch"
              <<"char"<<"class"<<"const"<<"continue"<<"default"<<"delete"<<"double"<<"else"<<"enum"
              <<"float"<<"for"<<"friend"<<"goto"<<"if"<<"inline"<<"int"<<"long"<<"new"<<"namespace"<<"operator"
              <<"private"<<"protected"<<"public"<<"register"<<"return"<<"short"<<"signed"<<"sizeof"<<"static"
              <<"struct"<<"switch"<<"template"<<"this"<<"throw"<<"try"<<"typedef"<<"union"<<"unsigned"
              <<"using"<<"virtual"<<"while";
    pykeylist<<"and"<<"del"<<"from"<<"not"<<"while"<<"as"<<"elif"<<"global"<<"with"
             <<"assert"<<"else"<<"pass"<<"yield"<<"break"<<"except"<<"import"<<"print"<<"class"<<"exec"<<"raise"<<"continue"
             <<"finally"<<"return"<<"for"<<"lambda"<<"try";
    return  true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("關於Formosa code"),
            tr("<b>Formosa code</b>是我在閒暇時間開發的一款文字編輯器<br>"
               "支援基本的語法高亮和自動完成程式碼<br>"
               "感謝您的使用<br>"));
}

void MainWindow::documentWasModified()
{
    setWindowModified(textEdit->isModified());
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&新增"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("新增檔案"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&開啟..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("開啟檔案"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&儲存"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("儲存檔案"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("另存新檔..."), this);
    saveAsAct->setShortcut(tr("Ctrl+Shift+S"));
    saveAsAct->setStatusTip(tr("將檔案儲存成其他名稱"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("離開"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("關閉程式"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(tr("剪下"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("剪下所選內容"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(tr("&複製"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("複製所選內容"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(tr("&貼上"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("貼上剪貼簿內容"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&關於"), this);
    aboutAct->setStatusTip(tr("關於本程式"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&檔案"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&編輯"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&幫助"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
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

bool MainWindow::maybeSave()
{
    if (textEdit->isModified()) {
        int ret = QMessageBox::warning(this, tr("警告"),
                     tr("變更尚未儲存\n"
                        "是否要儲存?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("警告"),
                             tr("無法載入檔案餒 %1 \n錯誤資訊:%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    SetupEditor(fileName.mid(fileName.lastIndexOf('.') + 1 ));

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setText(in.readAll());
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("檔案載入完成"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("警告"),
                             tr("無法寫入檔案 %1 \n 錯誤資訊:%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("儲存成功"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Formosa code")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
