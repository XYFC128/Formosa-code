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

#include "tab.h"

class FilelListView : public QWidget
{
Q_OBJECT
public:
         FilelListView();
    bool view(QString);
    void showFileInfoList(QFileInfoList list);
         ~FilelListView();

    Tab *editor;

private slots:
    void slotShow();
    void slotDirShow(QListWidgetItem *Item);

private:
    QFileInfoList   list;
    QLineEdit       *fileLineEdit;
    QListWidget     *fileListWidget;
    QVBoxLayout     *vLayout;
};

class fileViewer:public QDockWidget{
Q_OBJECT
public:
    fileViewer(Tab*);
    ~fileViewer();
private:
    FilelListView *fileview;
};

#endif // FILEVIEW_H
