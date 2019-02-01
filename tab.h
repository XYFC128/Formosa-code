#ifndef TAB_H
#define TAB_H
#include <QTabWidget>
#include "editor.h"
class Tab : public QTabWidget{
    Q_OBJECT
public:
    Tab();
    ~Tab();
    bool add(QString);
    bool save();
    bool saveAs();
    bool open();
private slots:
    void documentWasModified();
    void close(int);
private:
    QList <codeEditor*> editorList;
    void delTab(int);
    codeEditor* getCurrentEditor();
};

#endif // TAB_H
