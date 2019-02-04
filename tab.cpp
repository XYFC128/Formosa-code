#include <QFileDialog>
#include <QTextStream>
#include <QMimeData>
#include "tab.h"

Tab :: Tab(){
    this->setTabsClosable(true);
    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(close(int)));
    setAcceptDrops(true);
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
    creatAction(editor);
    return true;
}
void Tab::documentWasModified(){
    int index = this->currentIndex();
    QIcon icon;
    icon.addFile(":/image/dot.png");
    setTabIcon(index,icon);
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
bool Tab::closeAll(){
    foreach(codeEditor *editor,editorList){
        editor->maybeSave();
    }
    return true;
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
    QIcon icon;
    icon.addFile(":/image/null.png");
    setTabIcon(this->currentIndex(),icon);
    return true;
}
bool Tab::saveAs(){
    QString file = QFileDialog::getSaveFileName(this);
    if(file.isEmpty()){
        return false;
    }
    getCurrentEditor()->saveAs(file);
    QIcon icon;
    icon.addFile(":/image/null.png");
    setTabIcon(this->currentIndex(),icon);
    return true;
}
bool Tab::open(){
    return add(QFileDialog::getOpenFileName(this));
}

void Tab::cut(){
    getCurrentEditor()->cut();
}
void Tab::copy(){
    getCurrentEditor()->copy();
}
void Tab::paste(){
    getCurrentEditor()->paste();
}
void Tab::creatAction(codeEditor *editor){

    cutAct = new QAction(tr("剪下"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("剪下所選內容"));
    connect(cutAct, SIGNAL(triggered()), editor, SLOT(cut()));

    copyAct = new QAction(tr("複製"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("複製所選內容"));
    connect(copyAct, SIGNAL(triggered()), editor, SLOT(copy()));

    pasteAct = new QAction(tr("貼上"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("貼上剪貼簿內容"));
    connect(pasteAct, SIGNAL(triggered()), editor, SLOT(paste()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(editor, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(textChanged()),this, SLOT(documentWasModified()));
}

void Tab::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}
void Tab::dropEvent(QDropEvent *event){
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty()) {
        return;
    }
    add(fileName);
}
