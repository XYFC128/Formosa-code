#include "fileview.h"
#include<QTextCodec>
#include<QLabel>
#include<QMessageBox>
fileViewer::fileViewer(codeEditor *editor){
    fileview = new FilelListView;
    if(editor == nullptr) throw;
    this->setWindowTitle("檔案瀏覽");
    this->setFeatures(QDockWidget::AllDockWidgetFeatures);
    this->setAllowedAreas(Qt::RightDockWidgetArea|Qt::LeftDockWidgetArea);
    fileview->editor = editor;
    this->setWidget(fileview);
}
fileViewer::~fileViewer(){
    delete fileview;
}
FilelListView::FilelListView()
{
    //初始化指標
    fileLineEdit = new QLineEdit("/", this);
    fileListWidget = new QListWidget(this);

    //---布局
    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(fileLineEdit);
    vLayout->addWidget(fileListWidget);

    //--綁定事件
    connect(fileLineEdit, SIGNAL(returnPressed()),
                this, SLOT(slotShow()));
    connect(fileListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
                this, SLOT(slotDirShow(QListWidgetItem*)));
    //預設開啟資料夾為home
    if(QSysInfo::kernelType() == "winnt"){
        view("C:\\users");
    }
    else {
        view("/home");
    }
    setLayout(vLayout);
}
FilelListView::~FilelListView(){
    delete fileLineEdit;
    delete fileListWidget;
    delete vLayout;
    delete editor;
}
bool FilelListView::view(QString startStr){
    QDir dir(startStr);
    QStringList stringList;
    stringList << "*";
    QFileInfoList InfoList = dir.entryInfoList(stringList, QDir :: AllEntries, QDir :: DirsFirst);
    showFileInfoList(InfoList);
    fileLineEdit->setText(startStr);
    return true;
}
//顯示目前目錄中的文件
void FilelListView::slotShow()
{
     QDir dir(fileLineEdit->text());
     QStringList stringList;
     stringList << "*";
     QFileInfoList InfoList = dir.entryInfoList(stringList, QDir :: AllEntries, QDir :: DirsFirst);
     showFileInfoList(InfoList);
}

//雙擊事件
void FilelListView::showFileInfoList(QFileInfoList list)
{
    //清空列表控件
    fileListWidget->clear();

    //取出所有项，按照目录，文件方式添加到控件内
    for (unsigned int i = 0; i < list.count(); i++)
    {
        QFileInfo tmpFileInfo = list.at(i);
        if(tmpFileInfo.fileName()=="."){
            continue;
        }
        else if (tmpFileInfo.fileName()=="..") {
            QIcon icon(":/image/back.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem*tmpListWidgetItem = new QListWidgetItem(icon,fileName);
            fileListWidget->addItem(tmpListWidgetItem);
        }
        else if (tmpFileInfo.isDir())
        {
            QIcon icon(":/image/folder.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem*tmpListWidgetItem = new QListWidgetItem(icon,fileName);
            fileListWidget->addItem(tmpListWidgetItem);
        }
        else
        {
            QIcon icon(":/image/file.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem*tmpListWidgetItem = new QListWidgetItem(icon,fileName);
            fileListWidget->addItem(tmpListWidgetItem);
        }
    }
}

//----根据用户的选择显示下一级目录下的文件，
void FilelListView::slotDirShow(QListWidgetItem *Item)
{
    //----保存下一级目录名
    QString string = Item->text();
    QDir dir;
    //----设置路径为当前目录路径
    dir.setPath(fileLineEdit->text());
    //-----重新设置路径
    QFileInfo FileInfo(fileLineEdit->text()+"/"+string);
    if(FileInfo.isFile()) {
        editor->open(fileLineEdit->text()+"/"+string);
    }
    else {
        dir.cd(string);
        //----更新当前显示路径， 这里获取的是绝对路径
        fileLineEdit->setText(dir.absolutePath());
        //---显示当前文件目录下的所有文件
        slotShow();
    }
}
