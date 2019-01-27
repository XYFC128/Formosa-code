#ifndef EDITOR_H
#define EDITOR_H
#include <Qsci/qsciscintilla.h>
class codeEditor : public QsciScintilla{
public:
    codeEditor();
    ~codeEditor();
    void loadFile(const QString &fileName);
    bool saveFile();
    bool maybeSave();
    bool saveAs(const QString &fileName);
    void newFile();

    QString title;
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    bool SetupEditor();
    bool SetupKeyList();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QStringList cppkeylist,pykeylist;
    QString curFile;
};

#endif // EDITOR_H
