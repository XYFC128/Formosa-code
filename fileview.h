#ifndef FILEVIEW_H
#define FILEVIEW_H
#include <QWidget>
#include <QDockWidget>
#include <QDir>
#include <QListWidgetItem>
#include <QFileInfoList>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QIcon>
#include <QStringList>

#include "editor.h"

class FilelListView : public QWidget
{
Q_OBJECT
public:
    FilelListView();
    ~FilelListView();
    void showFileInfoList(QFileInfoList list);
    codeEditor *editor;
    bool view(QString);

private slots:
    void slotShow();
    void slotDirShow(QListWidgetItem *Item);

private:
    QLineEdit *fileLineEdit;
    QListWidget *fileListWidget;
    QVBoxLayout *vLayout;
    QFileInfoList list;
};

class fileViewer:public QDockWidget{
Q_OBJECT
public:
    fileViewer(codeEditor*);
    ~fileViewer();
private:
    FilelListView *fileview;
};

#endif // FILEVIEW_H
