#include "editor.h"
#include <QApplication>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>
codeEditor::codeEditor(){
    SetupKeyList();
    SetupEditor();
    setAcceptDrops(false);
}
codeEditor::~codeEditor(){
    if(textLexer != nullptr) delete textLexer;
}
bool codeEditor::SetupEditor(){
    this->setMarginType(4,QsciScintilla::NumberMargin);
    this->setMarginLineNumbers(4,true);
    this->setMarginWidth(4,31);

    this->setAutoCompletionSource(QsciScintilla::AcsAll);
    this->setAutoCompletionThreshold(2);
    this->setAutoCompletionCaseSensitivity(true);
    this->setFont(QFont("Courier New"));
    this->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);
    setCurrentFile("");

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
bool codeEditor::save()
{
    if(curFile.isEmpty()){
        return false;
    }
    QFile file(curFile);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("警告"),
                             tr("無法寫入檔案 %1:%2")
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
bool codeEditor::saveAs(QString fileName)
{
    if(fileName.isEmpty()){
        return false;
    }
    curFile = fileName;
    save();
    return true;
}
bool codeEditor::loadFile(const QString &fileName)
{
    if(this->isModified()){
        if(!maybeSave()){
            return false;
        }
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("警告"),
                             tr("無法載入檔案 %1:%2")
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
    QString shownName;
    if (curFile.isEmpty()){
        shownName = "untitled.txt";
    }
    else{
        shownName = strippedName(curFile);
    }


    QString s = shownName.mid(shownName.lastIndexOf('.') + 1 );
    if(s == "cpp"||s == "h"){
        textLexer = new QsciLexerCPP;
        this->setLexer(textLexer);
        QsciAPIs *apis = new QsciAPIs(textLexer);
        foreach(const QString &keyword,cppkeylist){
            apis->add(keyword);
        }
        apis->prepare();
    }
    else if (s == "py") {
        textLexer = new QsciLexerPython;
        this->setLexer(textLexer);
        QsciAPIs *apis = new QsciAPIs(textLexer);
        foreach(const QString &keyword,pykeylist){
            apis->add(keyword);
        }
        apis->prepare();
    }
    else{
        this->setLexer(0);
    }
    title = QString("%1").arg(shownName);
}
QString codeEditor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
void codeEditor::newFile(){
    this->clear();
    this->setModified(false);
    this->setCurrentFile("");
}
bool codeEditor::maybeSave(){
    if (this->isModified()) {
        int ret = QMessageBox::warning(this, tr("警告"),
                                       tr("文件%1的變更尚未儲存\n是否要儲存?").arg(curFile),
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
