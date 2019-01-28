#include "editor.h"
#include <QApplication>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QUrl>
#include <QMimeData>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>
codeEditor::codeEditor(){
    SetupKeyList();
    SetupEditor();
    setAcceptDrops(true);
}
codeEditor::~codeEditor(){

}
bool codeEditor::SetupEditor(){
    this->setMarginType(0,QsciScintilla::NumberMargin);
    this->setMarginLineNumbers(0,true);
    this->setMarginWidth(0,31);

    this->setAutoCompletionSource(QsciScintilla::AcsAll);
    //this->setAutoCompletionCaseSensitivity(false);
    this->setAutoCompletionThreshold(2);
    this->setFont(QFont("Courier New"));
    this->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);
    return true;
}
bool codeEditor::SetupKeyList(){
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
bool codeEditor::saveFile()
{
    QFile file(curFile);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("警告"),
                             tr("無法寫入檔案 %1 \n 錯誤資訊:%2.")
                             .arg(curFile)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << this->text();
    QApplication::restoreOverrideCursor();
    this->setModified(false);
    setCurrentFile(curFile);
    return true;
}
bool codeEditor::saveAs(const QString &fileName)
{
    if(fileName.isEmpty()){
        return false;
    }
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
    out << this->text();
    QApplication::restoreOverrideCursor();
    this->setModified(false);
    setCurrentFile(fileName);
    return true;
}
bool codeEditor::loadFile(const QString &fileName)
{
    if(this->isModified()){
        if(!maybeSave()){
            return false;
        }
    }
    this->clear();
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("警告"),
                             tr("無法載入檔案餒 %1:%2")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    setCurrentFile(fileName);
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->setText(in.readAll());
    this->setModified(false);
    QApplication::restoreOverrideCursor();
    return true;
}
void codeEditor::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    QString s = fileName.mid(fileName.lastIndexOf('.') + 1 );

    if(s == "cpp"||s == "h"){
        QsciLexerCPP *textLexer = new QsciLexerCPP;
        this->setLexer(textLexer);
        QsciAPIs *apis = new QsciAPIs(textLexer);
        foreach(const QString &keyword,cppkeylist){
            apis->add(keyword);
        }
        apis->prepare();
    }
    else if (s == "py") {
        QsciLexerPython *textLexer = new QsciLexerPython;
        this->setLexer(textLexer);
        QsciAPIs *apis = new QsciAPIs(textLexer);
        foreach(const QString &keyword,pykeylist){
            apis->add(keyword);
        }
        apis->prepare();
    }
    QString shownName;
    if (curFile.isEmpty()){
        shownName = "untitled.txt";
    }
    else{
        shownName = strippedName(curFile);
    }
    title = QString("%1 [*] - %2").arg(shownName).arg("Formosa code");
}
QString codeEditor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
void codeEditor::newFile(){
    this->clear();
    this->setModified(true);
    this->setCurrentFile("");
}
void codeEditor::dragEnterEvent(QDragEnterEvent *event){
     event->acceptProposedAction();
}

void codeEditor::dropEvent(QDropEvent *event){
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty()) {
        return;
    }
    this->loadFile(fileName);
}

bool codeEditor::maybeSave(){
    if (this->isModified()) {
        int ret = QMessageBox::warning(this, tr("警告"),
                     tr("變更尚未儲存\n"
                        "是否要儲存?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return saveFile();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
