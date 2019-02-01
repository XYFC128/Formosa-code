#ifndef EDITOR_H
#define EDITOR_H
#include <Qsci/qsciscintilla.h>
class codeEditor : public QsciScintilla{
public:
    codeEditor();
    ~codeEditor();
    bool save();
    bool maybeSave();
    bool saveAs(QString);
    void newFile();
    bool loadFile(const QString &fileName);

    QString title;
    QString curFile;
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    bool SetupEditor();
    bool SetupKeyList();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QsciLexer *textLexer;
    QStringList cppkeylist,pykeylist;
};

#endif // EDITOR_H
