#include <QFileDialog>
#include "tab.h"
Tab :: Tab(){
    this->setTabsClosable(true);
    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(close(int)));
    add("");
}
Tab :: ~Tab(){

}
bool Tab::add(QString file){
    int index = this->currentIndex()+1;
    codeEditor *editor = new codeEditor;
    if(file.isEmpty()){
        editor->newFile();
    }
    else {
        editor->loadFile(file);
    }
    this->insertTab(index,editor,editor->title);
    this->setCurrentIndex(index);
    editorList.insert(index,editor);
    connect(editor, SIGNAL(textChanged()),this, SLOT(documentWasModified()));
    return true;
}
void Tab::documentWasModified(){
    int index = this->currentIndex();
    if(this->tabText(index).endsWith("*",Qt::CaseInsensitive)){
        return;
    }
    else {
        this->setTabText(index,this->tabText(index)+"*");
    }
}
void Tab::delTab(int i){
    this->removeTab(i);
    editorList.removeAt(i);
}
void Tab::close(int index){
    if (editorList[index]->isModified()) {
        if(editorList[index]->maybeSave()){
            delTab(index);
        }
        else {
            return;
        }
    } else {
        delTab(index);
    }

    if(this->count() == 0){
        add("");
    }
}
codeEditor* Tab::getCurrentEditor(){
    int index = this->currentIndex();
    return editorList[index];
}
bool Tab::save(){
    if(!getCurrentEditor()->save()){
        return saveAs();
    }
    QString s = this->tabText(this->currentIndex());
    if(s.endsWith("*")){
        this->setTabText(this->currentIndex(),s.left(s.length()-1));
    }
    return true;
}
bool Tab::saveAs(){
    getCurrentEditor()->saveAs(QFileDialog::getSaveFileName(this));
    QString s = this->tabText(this->currentIndex());
    if(s.endsWith("*")){
        this->setTabText(this->currentIndex(),s.left(s.length()-1));
    }
    return true;
}
bool Tab::open(){
    return add(QFileDialog::getOpenFileName(this));
}
