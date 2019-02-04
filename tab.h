#ifndef TAB_H
#define TAB_H
#include <QTabWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QAction>
#include "editor.h"
class Tab : public QTabWidget{
    Q_OBJECT
public:
         Tab();
    bool add(QString);
    bool closeAll();
    bool open();
    bool save();
    bool saveAs();
         ~Tab();

public slots:
    void copy();
    void cut();
    void paste();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void documentWasModified();
    void close(int);

private:
    void delTab(int);
    void creatAction(codeEditor*);

    QAction             *copyAct;
    QAction             *cutAct;
    QAction             *pasteAct;
    QList <codeEditor*> editorList;
    codeEditor*         getCurrentEditor();
};

#endif // TAB_H
